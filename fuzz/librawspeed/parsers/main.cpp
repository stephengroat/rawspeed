/*
    RawSpeed - RAW file decoder.

    Copyright (C) 2017 Roman Lebedev

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef PARSER
#error PARSER must be defined
#endif

#ifndef GETDECODER
#error GETDECODER must be defined as bool
#endif

#include "io/Buffer.h"                   // for Buffer, DataBuffer
#include "parsers/CiffParser.h"          // IWYU pragma: keep
#include "parsers/CiffParserException.h" // IWYU pragma: keep
#include "parsers/FiffParser.h"          // IWYU pragma: keep
#include "parsers/FiffParserException.h" // IWYU pragma: keep
#include "parsers/RawParser.h"           // IWYU pragma: keep
#include "parsers/RawParserException.h"  // IWYU pragma: keep
#include "parsers/TiffParser.h"          // IWYU pragma: keep
#include "parsers/TiffParserException.h" // IWYU pragma: keep
#include "parsers/X3fParser.h"           // IWYU pragma: keep
#include "parsers/X3fParserException.h"  // IWYU pragma: keep
#include "tiff/TiffEntry.h"              // IWYU pragma: keep
#include <cassert>                       // for assert
#include <cstdint>                       // for uint8_t
#include <cstdio>                        // for size_t

#if GETDECODER
#include "decoders/RawDecoder.h" // IWYU pragma: keep
#endif

#define TOKENPASTE2(x, y) x##y
#define TOKENPASTE(x, y) TOKENPASTE2(x, y)

// define this function, it is only declared in rawspeed:
// for fuzzing, do not want any threading.
extern "C" int __attribute__((const)) rawspeed_get_number_of_processor_cores() {
  return 1;
}

using PARSERException = TOKENPASTE(rawspeed::PARSER, Exception);

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size);

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size) {
  assert(Data);

  const rawspeed::Buffer buffer(Data, Size);

  try {
    rawspeed::PARSER parser(&buffer);

#if GETDECODER
    parser.getDecoder();
#endif
  } catch (PARSERException&) {
    return 0;
  }

  return 0;
}