#pragma once

/*
 *      Copyright (C) 2005-2012 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "DVDVideoCodec.h"
#include "DVDStreamInfo.h"
#include "utils/BitstreamConverter.h"
#include "xbmc/linux/LinuxV4l2.h"

#ifndef V4L2_CAP_VIDEO_M2M_MPLANE
  #define V4L2_CAP_VIDEO_M2M_MPLANE 0x00004000
#endif

#define STREAM_BUFFER_SIZE            1048576 //compressed frame size buffer. for unknown reason, possibly the firmware bug,
                                              //if set to lower values it corrupts adjacent value in the setup data structure for h264 streams
                                              //and leads to stream hangs on heavy frames
#define FIMC_CAPTURE_BUFFERS_CNT      3 //2 begins to be slow.
#define MFC_OUTPUT_BUFFERS_CNT        3 //1 doesn't work at all, 2 is enough most of the times, but in a rare case of interlaced video two buffers
                                        //must be queued all the time to get fill picture from interlaced frames, so let's have them 3

#define memzero(x) memset(&(x), 0, sizeof (x))

class CDVDVideoCodecMFC : public CDVDVideoCodec
{
public:
  CDVDVideoCodecMFC();
  virtual ~CDVDVideoCodecMFC();
  virtual bool Open(CDVDStreamInfo &hints, CDVDCodecOptions &options);
  virtual void Dispose();
  virtual int Decode(BYTE* pData, int iSize, double dts, double pts);
  virtual void Reset();
  bool GetPictureCommon(DVDVideoPicture* pDvdVideoPicture);
  virtual bool GetPicture(DVDVideoPicture* pDvdVideoPicture);
  virtual bool ClearPicture(DVDVideoPicture* pDvdVideoPicture);
  virtual void SetDropState(bool bDrop);
  virtual const char* GetName() { return m_name.c_str(); }; // m_name is never changed after open

protected:
  std::string m_name;
  int m_iDecoderHandle;
  int m_iConverterHandle;

  int m_MFCOutputBuffersCount;
  int m_MFCCaptureBuffersCount;
  int m_FIMCCaptureBuffersCount;

  V4L2Buffer *m_v4l2MFCOutputBuffers;
  V4L2Buffer *m_v4l2MFCCaptureBuffers;
  V4L2Buffer *m_v4l2FIMCCaptureBuffers;

  int m_iDequeuedToPresentBufferNumber;

  bool m_bVideoConvert;
  CDVDStreamInfo m_hints;

  CBitstreamConverter m_converter;
  bool m_bDropPictures;

  DVDVideoPicture   m_videoBuffer;

  bool OpenDevices();
};
