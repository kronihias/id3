/******************************************************
 *
 * id3 - implementation file
 *
 * pure data external to read/write ID3 Tag
 * 
 * (C) Copyright 2011 Matthias Kronlachner
 *
 * Email: m.kronlachner@gmail.com
 *
 ******************************************************
 *
 * license: GNU General Public License v.2
 *
 ******************************************************/

#include "m_pd.h"
#include <id3.h>
#include <stdio.h>


// define field infos
static const char *ID3_fids[96] =
{
  /* ???? */ "ID3FID_NOFRAME",       /**< No known frame */
  /* AENC */ "ID3FID_AUDIOCRYPTO",       /**< Audio encryption */
  /* APIC */ "ID3FID_PICTURE",           /**< Attached picture */
  /* ASPI */ "ID3FID_AUDIOSEEKPOINT",    /**< Audio seek point index */
  /* COMM */ "ID3FID_COMMENT",           /**< Comments */
  /* COMR */ "ID3FID_COMMERCIAL",        /**< Commercial frame */
  /* ENCR */ "ID3FID_CRYPTOREG",         /**< Encryption method registration */
  /* EQU2 */ "ID3FID_EQUALIZATION2",     /**< Equalisation (2) */
  /* EQUA */ "ID3FID_EQUALIZATION",      /**< Equalization */
  /* ETCO */ "ID3FID_EVENTTIMING",       /**< Event timing codes */
  /* GEOB */ "ID3FID_GENERALOBJECT",     /**< General encapsulated object */
  /* GRID */ "ID3FID_GROUPINGREG",       /**< Group identification registration */
  /* IPLS */ "ID3FID_INVOLVEDPEOPLE",    /**< Involved people list */
  /* LINK */ "ID3FID_LINKEDINFO",        /**< Linked information */
  /* MCDI */ "ID3FID_CDID",              /**< Music CD identifier */
  /* MLLT */ "ID3FID_MPEGLOOKUP",        /**< MPEG location lookup table */
  /* OWNE */ "ID3FID_OWNERSHIP",         /**< Ownership frame */
  /* PRIV */ "ID3FID_PRIVATE",           /**< Private frame */
  /* PCNT */ "ID3FID_PLAYCOUNTER",       /**< Play counter */
  /* POPM */ "ID3FID_POPULARIMETER",     /**< Popularimeter */
  /* POSS */ "ID3FID_POSITIONSYNC",      /**< Position synchronisation frame */
  /* RBUF */ "ID3FID_BUFFERSIZE",        /**< Recommended buffer size */
  /* RVA2 */ "ID3FID_VOLUMEADJ2",        /**< Relative volume adjustment (2) */
  /* RVAD */ "ID3FID_VOLUMEADJ",         /**< Relative volume adjustment */
  /* RVRB */ "ID3FID_REVERB",            /**< Reverb */
  /* SEEK */ "ID3FID_SEEKFRAME",         /**< Seek frame */
  /* SIGN */ "ID3FID_SIGNATURE",         /**< Signature frame */
  /* SYLT */ "ID3FID_SYNCEDLYRICS",      /**< Synchronized lyric/text */
  /* SYTC */ "ID3FID_SYNCEDTEMPO",       /**< Synchronized tempo codes */
  /* TALB */ "ID3FID_ALBUM",             /**< Album/Movie/Show title */
  /* TBPM */ "ID3FID_BPM",               /**< BPM (beats per minute) */
  /* TCOM */ "ID3FID_COMPOSER",          /**< Composer */
  /* TCON */ "ID3FID_CONTENTTYPE",       /**< Content type */
  /* TCOP */ "ID3FID_COPYRIGHT",         /**< Copyright message */
  /* TDAT */ "ID3FID_DATE",              /**< Date */
  /* TDEN */ "ID3FID_ENCODINGTIME",      /**< Encoding time */
  /* TDLY */ "ID3FID_PLAYLISTDELAY",     /**< Playlist delay */
  /* TDOR */ "ID3FID_ORIGRELEASETIME",   /**< Original release time */
  /* TDRC */ "ID3FID_RECORDINGTIME",     /**< Recording time */
  /* TDRL */ "ID3FID_RELEASETIME",       /**< Release time */
  /* TDTG */ "ID3FID_TAGGINGTIME",       /**< Tagging time */
  /* TIPL */ "ID3FID_INVOLVEDPEOPLE2",   /**< Involved people list */
  /* TENC */ "ID3FID_ENCODEDBY",         /**< Encoded by */
  /* TEXT */ "ID3FID_LYRICIST",          /**< Lyricist/Text writer */
  /* TFLT */ "ID3FID_FILETYPE",          /**< File type */
  /* TIME */ "ID3FID_TIME",              /**< Time */
  /* TIT1 */ "ID3FID_CONTENTGROUP",      /**< Content group description */
  /* TIT2 */ "ID3FID_TITLE",             /**< Title/songname/content description */
  /* TIT3 */ "ID3FID_SUBTITLE",          /**< Subtitle/Description refinement */
  /* TKEY */ "ID3FID_INITIALKEY",        /**< Initial key */
  /* TLAN */ "ID3FID_LANGUAGE",          /**< Language(s) */
  /* TLEN */ "ID3FID_SONGLEN",           /**< Length */
  /* TMCL */ "ID3FID_MUSICIANCREDITLIST",/**< Musician credits list */
  /* TMED */ "ID3FID_MEDIATYPE",         /**< Media type */
  /* TMOO */ "ID3FID_MOOD",              /**< Mood */
  /* TOAL */ "ID3FID_ORIGALBUM",         /**< Original album/movie/show title */
  /* TOFN */ "ID3FID_ORIGFILENAME",      /**< Original filename */
  /* TOLY */ "ID3FID_ORIGLYRICIST",      /**< Original lyricist(s)/text writer(s) */
  /* TOPE */ "ID3FID_ORIGARTIST",        /**< Original artist(s)/performer(s) */
  /* TORY */ "ID3FID_ORIGYEAR",          /**< Original release year */
  /* TOWN */ "ID3FID_FILEOWNER",         /**< File owner/licensee */
  /* TPE1 */ "ID3FID_LEADARTIST",        /**< Lead performer(s)/Soloist(s) */
  /* TPE2 */ "ID3FID_BAND",              /**< Band/orchestra/accompaniment */
  /* TPE3 */ "ID3FID_CONDUCTOR",         /**< Conductor/performer refinement */
  /* TPE4 */ "ID3FID_MIXARTIST",         /**< Interpreted", remixed", or otherwise modified by */
  /* TPOS */ "ID3FID_PARTINSET",         /**< Part of a set */
  /* TPRO */ "ID3FID_PRODUCEDNOTICE",    /**< Produced notice */
  /* TPUB */ "ID3FID_PUBLISHER",         /**< Publisher */
  /* TRCK */ "ID3FID_TRACKNUM",          /**< Track number/Position in set */
  /* TRDA */ "ID3FID_RECORDINGDATES",    /**< Recording dates */
  /* TRSN */ "ID3FID_NETRADIOSTATION",   /**< Internet radio station name */
  /* TRSO */ "ID3FID_NETRADIOOWNER",     /**< Internet radio station owner */
  /* TSIZ */ "ID3FID_SIZE",              /**< Size */
  /* TSOA */ "ID3FID_ALBUMSORTORDER",    /**< Album sort order */
  /* TSOP */ "ID3FID_PERFORMERSORTORDER",/**< Performer sort order */
  /* TSOT */ "ID3FID_TITLESORTORDER",    /**< Title sort order */
  /* TSRC */ "ID3FID_ISRC",              /**< ISRC (international standard recording code) */
  /* TSSE */ "ID3FID_ENCODERSETTINGS",   /**< Software/Hardware and settings used for encoding */
  /* TSST */ "ID3FID_SETSUBTITLE",       /**< Set subtitle */
  /* TXXX */ "ID3FID_USERTEXT",          /**< User defined text information */
  /* TYER */ "ID3FID_YEAR",              /**< Year */
  /* UFID */ "ID3FID_UNIQUEFILEID",      /**< Unique file identifier */
  /* USER */ "ID3FID_TERMSOFUSE",        /**< Terms of use */
  /* USLT */ "ID3FID_UNSYNCEDLYRICS",    /**< Unsynchronized lyric/text transcription */
  /* WCOM */ "ID3FID_WWWCOMMERCIALINFO", /**< Commercial information */
  /* WCOP */ "ID3FID_WWWCOPYRIGHT",      /**< Copyright/Legal infromation */
  /* WOAF */ "ID3FID_WWWAUDIOFILE",      /**< Official audio file webpage */
  /* WOAR */ "ID3FID_WWWARTIST",         /**< Official artist/performer webpage */
  /* WOAS */ "ID3FID_WWWAUDIOSOURCE",    /**< Official audio source webpage */
  /* WORS */ "ID3FID_WWWRADIOPAGE",      /**< Official internet radio station homepage */
  /* WPAY */ "ID3FID_WWWPAYMENT",        /**< Payment */
  /* WPUB */ "ID3FID_WWWPUBLISHER",      /**< Official publisher webpage */
  /* WXXX */ "ID3FID_WWWUSER",           /**< User defined URL link */
  /*      */ "ID3FID_METACRYPTO",        /**< Encrypted meta frame (id3v2.2.x) */
  /*      */ "ID3FID_METACOMPRESSION",   /**< Compressed meta frame (id3v2.2.1) */
  /* >>>> */ "ID3FID_LASTFRAMEID "       /**< Last field placeholder */
};


typedef struct _id3
{
  t_object x_obj;
	
	int tag_ready; //is a valid file opened??
	ID3Tag *id3_tag;    /* Tag defined by the id3lib */
	ID3Frame *id3_frame, *id3_first_frame;

  t_outlet*x_infoout;
} t_id3;

/* ------------------------ id3 ----------------------------- */ 

static t_class *id3_class;


  ///////////////////////////////
 /////      OPEN FILE      /////
///////////////////////////////
static void id3_open(t_id3 *x, t_symbol*s, int argc, t_atom*argv) {
	
	ID3Field id3_field;
  size_t offset;

	    
	unsigned char tmp[4];
  int has_id3v1_tag = 1;
  int has_id3v2_tag = 1;
  int file_available = 0;
	FILE *file;
	
	// TRY TO OPEN FILE
	  if ((file=fopen(s->s_name,"rwb"))==NULL)
    {
			if ((file=fopen(s->s_name,"rwb"))==NULL)
			{
				post("ID3:: Error opening file: '%s'", s->s_name);
			} else {
				post("ID3:: File is read-only: '%s'", s->s_name);
				file_available = 1;
			}
    } else {
			post("ID3:: File is read-and-writeable: '%s'", s->s_name);
			file_available = 1;
		}
		
		if (file_available)
		{
		
		// CHECK IF TAG EXISTS
		
			if (fread(tmp, 1, 4, file) != 4)
			{
					fclose(file);
			}
			if (tmp[0] != 'I' || tmp[1] != 'D' || tmp[2] != '3' || tmp[3] >= 0xFF) // ID3v2 tag skipeer $49 44 33 yy yy xx zz zz zz zz [zz size]
			{
					// ID3v2 tag not found!
					has_id3v2_tag = 0;
					post("ID3:: no ID3v2 tag found!!");
			} else {
					post("ID3:: ID3v2 tag found!!");
			}
			// 2) ID3v1 tag
			if (fseek(file,-128, SEEK_END)!=0 // Go to the beginning of ID3v1 tag
			||  fread(tmp,1,3,file) != 3)
			{
					fclose(file);
			}
			if (tmp[0] != 'T' || tmp[1] != 'A' || tmp[2] != 'G')
			{
					// ID3v1 tag not found!
					has_id3v1_tag = 0;
					post("ID3:: NO ID3v1 tag found!!");
			} else {
					post("ID3:: ID3v1 tag found!!");
			}
			fclose(file); // We close it cause id3lib opens/closes file itself

			/* Get data from tag */
			if ( (x->id3_tag = ID3Tag_New()) == NULL )
					post("ID3:: couldn't create empty tag");

			/* Link the file to the tag */
			offset = ID3Tag_Link(x->id3_tag,s->s_name);
			
			x->tag_ready = 1; //FILE IS READY!
		}
    
}


  ///////////////////////////////
 ///// GET TAG INFORMATION /////
///////////////////////////////
static void id3_get(t_id3 *x, t_symbol*s, int argc, t_atom*argv) {
	
	if (x->tag_ready)
	{
		ID3Field *id3_field = NULL;
			
		char  *string, *string1, *string2;

		char text[200];
		
		if (gensym("title")==s)
		{
			/****************
			 * Title (TIT2) *
			 ****************/
			if ( (x->id3_frame = ID3Tag_FindFrameWithID(x->id3_tag,ID3FID_TITLE)) )
			{
					id3_field = ID3Frame_GetField(x->id3_frame,ID3FN_TEXT);
			}
		}

		if (gensym("artist")==s)
		{
			/*****************
			 * Artist (TPE1) *
			 *****************/
			if ( (x->id3_frame = ID3Tag_FindFrameWithID(x->id3_tag,ID3FID_LEADARTIST)) )
			{
					id3_field = ID3Frame_GetField(x->id3_frame,ID3FN_TEXT);
			}
		}


		if (gensym("album")==s)
		{
			/****************
			 * Album (TALB) *
			 ****************/
			if ( (x->id3_frame = ID3Tag_FindFrameWithID(x->id3_tag,ID3FID_ALBUM)) )
			{
					id3_field = ID3Frame_GetField(x->id3_frame,ID3FN_TEXT);
			}
		}
		
		if (gensym("comment")==s)
		{
			/******************
			 * Comment (COMM) *
			 ******************/
			if ( (x->id3_frame = ID3Tag_FindFrameWithID(x->id3_tag,ID3FID_COMMENT)) )
			{
					id3_field = ID3Frame_GetField(x->id3_frame,ID3FN_TEXT);
			}
		}
			
			if (id3_field)
			{
				ID3Field_GetASCII(id3_field,text,sizeof(text));
				
				t_atom ap[1];
				
				SETSYMBOL(ap, gensym(text));
				outlet_anything(x->x_infoout, s, 1, ap);
			}
		} else {
			post("ID3:: no file with supported id3 tag opened");
		}
}


  ///////////////////////////////
 ///// SET TAG INFORMATION /////
///////////////////////////////
static void id3_set(t_id3 *x, t_symbol*s, int argc, t_atom*argv) {
	
			t_binbuf*bb=binbuf_new();
			int size=0;
			char*text=NULL;

			binbuf_add(bb, argc-1, argv+1);
			binbuf_gettext(bb, &text, &size);
			binbuf_free(bb);
			text[size]=0;
			
			t_binbuf*bb2=binbuf_new();
			char*field=NULL;
			binbuf_add(bb2, 1, argv);
			binbuf_gettext(bb2, &field, &size);
			binbuf_free(bb2);
			field[size]=0;			
			
			//post("ID3:: set field %s to %s", atom_getsymbol(argv), text);
			
	if (x->tag_ready)
	{
		ID3Field *id3_field = NULL;
		
		//char  *string, *string1, *string2;
		
		//char text[200];
		
		if (gensym("title")==atom_getsymbol(argv))
		{
			/****************
			 * Title (TIT2) *
			 ****************/
			if ( (x->id3_frame = ID3Tag_FindFrameWithID(x->id3_tag,ID3FID_TITLE)) )
			{
				id3_field = ID3Frame_GetField(x->id3_frame,ID3FN_TEXT);
			}
		}
		
		if (gensym("artist")==atom_getsymbol(argv))
		{
			post("debug haha");
			/*****************
			 * Artist (TPE1) *
			 *****************/
			if ( (x->id3_frame = ID3Tag_FindFrameWithID(x->id3_tag,ID3FID_LEADARTIST)) )
			{
				id3_field = ID3Frame_GetField(x->id3_frame,ID3FN_TEXT);
			}
		}
		
		
		if (gensym("album")==atom_getsymbol(argv))
		{
			/****************
			 * Album (TALB) *
			 ****************/
			if ( (x->id3_frame = ID3Tag_FindFrameWithID(x->id3_tag,ID3FID_ALBUM)) )
			{
				id3_field = ID3Frame_GetField(x->id3_frame,ID3FN_TEXT);
			}
		}
		
		if (gensym("comment")==atom_getsymbol(argv))
		{
			/******************
			 * Comment (COMM) *
			 ******************/
			if ( (x->id3_frame = ID3Tag_FindFrameWithID(x->id3_tag,ID3FID_COMMENT)) )
			{
				id3_field = ID3Frame_GetField(x->id3_frame,ID3FN_TEXT);
			}
		}
		
		if (gensym("raw")==atom_getsymbol(argv))
		{
			
			post("ID3:: this function is not working in the moment.... sorry!");
			/******************
			 * RAW *
			 ******************/
			t_binbuf*bb3=binbuf_new();
			char*raw=NULL;
			binbuf_add(bb3, 1, argv+1);
			binbuf_gettext(bb3, &raw, &size);
			binbuf_free(bb3);
			raw[size]=0;			
			
			t_binbuf*bb4=binbuf_new();
			binbuf_add(bb4, argc-2, argv+2);
			binbuf_gettext(bb4, &text, &size);
			binbuf_free(bb4);
			text[size]=0;
			
			 //post("ID3:: set %s to %s", raw, text);
			if ( (x->id3_frame = ID3Tag_FindFrameWithID(x->id3_tag, (ID3_FrameID) raw)) )
			{
				id3_field = ID3Frame_GetField(x->id3_frame,ID3FN_TEXT);
			} else {
				post("ID3:: no such field: %s", raw);
			}
		}
		
		if (id3_field)
		{
			post("ID3:: set field %s to %s", field, text);
			ID3Field_SetASCII(id3_field,text); // UPDATE FIELD
		}
	} else {
		post("ID3:: no file with supported id3 tag opened");
	}
}


	///////////////////////////////
 /////     UPDATE FILE      ////
///////////////////////////////
static void id3_update(t_id3*x) {
	if (x->tag_ready)
	{
		if (!(int)ID3Tag_Update(x->id3_tag))
		{
			post("ID3:: tag written to file");
		} else {
			post("ID3:: error writing to file");
		}
		} else {
			post("ID3:: no file with supported id3 tag opened");
	}
}


  ///////////////////////////////
 ///// BANG RAW INFORMATION ////
///////////////////////////////

static void id3_info(t_id3*x) {
	// OUTPUT ALL IF BANGED??
	
	ID3Field *id3_field = NULL;
	char  *string, *string1, *string2;
	char text[200];
	char *id_name;
	
	ID3TagIterator *iter = ID3Tag_CreateIterator(x->id3_tag);
	ID3Frame* myFrame = NULL;
	t_atom ap[2];
	ID3_FrameID id;
	
	while (NULL != (myFrame = ID3TagIterator_GetNext(iter)))
	{
		id=ID3Frame_GetID(myFrame);
		id3_field = ID3Frame_GetField(myFrame,ID3FN_TEXT);
		ID3Field_GetASCII(id3_field,text,sizeof(text));
		//int num = ID3FrameInfo_NumFields(id);
		
		//id_name = ID3FrameInfo_ShortName(id);
		SETSYMBOL(ap+1, gensym(text));
		SETSYMBOL(ap+0, gensym(ID3_fids[id]));
		
		
		// post("ID: %s  TEXT: %s", ID3_fids[id], text);
		
		outlet_anything(x->x_infoout, gensym("raw"), 2, ap);
	}
}


static void *id3_new(void){
  t_id3 *x = (t_id3 *)pd_new(id3_class);
  
  x->tag_ready = 0;
  
  post("::::::::::::");
	post("ID3 pd external");
	post("(C) 2011 Matthias Kronlachner");
  post("::::::::::::");

  x->x_infoout=outlet_new(&x->x_obj, &s_symbol);

  return (x);
}

void id3_setup(void){

  id3_class = class_new(gensym("id3"), (t_newmethod)id3_new, 0, sizeof(t_id3), 0, 0);

  class_addmethod(id3_class, (t_method)id3_open, gensym("open"), A_SYMBOL, 0);

  class_addmethod(id3_class, (t_method)id3_get, gensym("get"), A_SYMBOL, 0);

	class_addmethod(id3_class, (t_method)id3_set, gensym("set"), A_GIMME, 0);
	
	class_addmethod(id3_class, (t_method)id3_update, gensym("update"), 0);

  class_addbang(id3_class, (t_method)id3_info);

}
