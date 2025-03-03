#include "AAS.h"
#include "../AAS_Data.h"
#include "tonc.h"

#include "fotocopia.h"

#define REG_KEY (*(volatile AAS_u16 *)0x04000130)
#define REG_KEY_A 0x0001
#define REG_KEY_B 0x0002

void init_main()
{
	REG_DISPCNT= DCNT_MODE4 | DCNT_BG2;
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
  irq_add(II_TIMER1, AAS_Timer1InterruptHandler);
  AAS_SetConfig(AAS_CONFIG_MIX_32KHZ, AAS_CONFIG_CHANS_8, AAS_CONFIG_SPATIAL_STEREO, AAS_CONFIG_DYNAMIC_OFF);
}

int main()
{
  const AAS_s8 *const album_start[6] = {
      AAS_DATA_SFX_START_fotocopia_001,
      AAS_DATA_SFX_START_fotocopia_002,
      AAS_DATA_SFX_START_fotocopia_003,
      AAS_DATA_SFX_START_fotocopia_004,
      AAS_DATA_SFX_START_fotocopia_005,
      AAS_DATA_SFX_START_fotocopia_006};
  
  const AAS_s8 *const album_end[6] = {
      AAS_DATA_SFX_END_fotocopia_001,
      AAS_DATA_SFX_END_fotocopia_002,
      AAS_DATA_SFX_END_fotocopia_003,
      AAS_DATA_SFX_END_fotocopia_004,
      AAS_DATA_SFX_END_fotocopia_005,
      AAS_DATA_SFX_END_fotocopia_006};
  
  int SongIndex = 0;

  int keys, keys_changed;
  int prev_keys = 0;
  init_main();
    
  memcpy32(vid_mem, fotocopiaBitmap, fotocopiaBitmapLen / sizeof(u32));
  memcpy16(pal_bg_mem, fotocopiaPal, fotocopiaPalLen / sizeof(u16));

  while(1)
  {
    vid_vsync();
   
    keys = ~REG_KEY;
    keys_changed = keys ^ prev_keys;
    prev_keys = keys;
  
    if ((keys_changed & REG_KEY_A) && (keys & REG_KEY_A)) {
        AAS_SFX_Play(0, 64, 8000, album_start[SongIndex], album_end[SongIndex], AAS_NULL);
        AAS_SFX_Play(1, 64, 8000, album_start[SongIndex], album_end[SongIndex], AAS_NULL);
    }

    if ((keys_changed & REG_KEY_B) && (keys & REG_KEY_B)) {
      SongIndex = (SongIndex + 1) % 6;
    }

    VBlankIntrWait();
  }

  return 0;
}
