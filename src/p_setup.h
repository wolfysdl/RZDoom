// Emacs style mode select	 -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// DESCRIPTION:
//	 Setup a game, startup stuff.
//
//-----------------------------------------------------------------------------


#ifndef __P_SETUP__
#define __P_SETUP__

#include "w_wad.h"
#include "doomdata.h"


struct MapData
{
	wadlump_t MapLumps[ML_MAX];
	bool HasBehavior;
	bool CloseOnDestruct;
	bool Encrypted;
	bool isText;
	int lumpnum;
	FileReader * file;
	
	MapData()
	{
		memset(MapLumps, 0, sizeof(MapLumps));
		file = NULL;
		lumpnum = -1;
		HasBehavior = false;
		CloseOnDestruct = true;
		Encrypted = false;
		isText = false;
	}
	
	~MapData()
	{
		if (CloseOnDestruct && file != NULL) delete file;
		file = NULL;
	}

	void Seek(unsigned int lumpindex)
	{
		if (lumpindex<countof(MapLumps))
		{
			file->Seek(MapLumps[lumpindex].FilePos, SEEK_SET);
		}
	}

	void Read(unsigned int lumpindex, void * buffer, int size = -1)
	{
		if (lumpindex<countof(MapLumps))
		{
			if (size == -1) size = MapLumps[lumpindex].Size;
			file->Seek(MapLumps[lumpindex].FilePos, SEEK_SET);
			file->Read(buffer, size);
		}
	}

	DWORD Size(unsigned int lumpindex)
	{
		if (lumpindex<countof(MapLumps))
		{
			return MapLumps[lumpindex].Size;
		}
		return 0;
	}

	void GetChecksum(BYTE cksum[16]);
};

MapData * P_OpenMapData(const char * mapname);
bool P_CheckMapData(const char * mapname);


// NOT called by W_Ticker. Fixme. [RH] Is that bad?
//
// [RH] The only parameter used is mapname, so I removed playermask and skill.
//		On September 1, 1998, I added the position to indicate which set
//		of single-player start spots should be spawned in the level.
void P_SetupLevel (char *mapname, int position);

void P_FreeLevelData();
void P_FreeExtraLevelData();

// Called by startup code.
void P_Init (void);

struct line_t;
struct maplinedef_t;

void P_LoadTranslator(const char *lumpname);
void P_TranslateLineDef (line_t *ld, maplinedef_t *mld);
int P_TranslateSectorSpecial (int);

int GetUDMFInt(int type, int index, const char *key);
fixed_t GetUDMFFixed(int type, int index, const char *key);

bool P_LoadGLNodes(MapData * map);
bool P_CheckNodes(MapData * map, bool rebuilt, int buildtime);
bool P_CheckForGLNodes();
void P_SetRenderSector();


struct sidei_t	// [RH] Only keep BOOM sidedef init stuff around for init
{
	union
	{
		// Used when unpacking sidedefs and assigning
		// properties based on linedefs.
		struct
		{
			short tag, special;
			short alpha;
			DWORD map;
		} a;

		// Used when grouping sidedefs into loops.
		struct
		{
			DWORD first, next;
			char lineside;
		} b;
	};
};
extern sidei_t *sidetemp;
extern bool hasglnodes;
extern struct glsegextra_t *glsegextras;

struct FMissingCount
{
	FMissingCount() : Count(0) {}
	int Count;
};
typedef TMap<FString,FMissingCount> FMissingTextureTracker;

#endif
