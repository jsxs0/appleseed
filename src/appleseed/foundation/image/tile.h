
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010-2013 Francois Beaune, Jupiter Jazz Limited
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef APPLESEED_FOUNDATION_IMAGE_TILE_H
#define APPLESEED_FOUNDATION_IMAGE_TILE_H

// appleseed.foundation headers.
#include "foundation/core/concepts/noncopyable.h"
#include "foundation/image/pixel.h"
#include "foundation/platform/types.h"

// appleseed.main headers.
#include "main/dllsymbol.h"

// Standard headers.
#include <cassert>
#include <cstddef>
#include <cstring>

namespace foundation
{

//
// A tile, as a 2D array of pixels.
//

class DLLSYMBOL Tile
  : public NonCopyable
{
  public:
    // Construct a new tile. The content of the tile is left undefined.
    Tile(
        const size_t        width,              // tile width, in pixels
        const size_t        height,             // tile height, in pixels
        const size_t        channel_count,      // number of channels
        const PixelFormat   pixel_format,       // pixel format
        uint8*              storage = 0);       // if provided, use this memory for pixel storage

    // Construct a tile by converting an existing tile to a given pixel format.
    Tile(
        const Tile&         tile,               // source tile
        const PixelFormat   pixel_format,       // new pixel format
        uint8*              storage = 0);       // if provided, use this memory for pixel storage

    // Construct a tile by converting an existing tile to a given pixel format,
    // and allowing reordering, replication and deletion of channels.
    Tile(
        const Tile&         tile,               // source tile
        const PixelFormat   pixel_format,       // new pixel format
        const size_t*       shuffle_table,      // channel shuffling table
        uint8*              storage = 0);       // if provided, use this memory for pixel storage

    // Copy constructor.
    Tile(const Tile& rhs);

    // Destructor.
    ~Tile();

    // Like foundation::IUnknown::release() but without introducing a virtual function table.
    void release();

    // Return the size (in bytes) of this object in memory.
    size_t get_memory_size() const;

    // Tile properties.
    PixelFormat get_pixel_format() const;
    size_t get_width() const;
    size_t get_height() const;
    size_t get_channel_count() const;           // number of channels in one pixel
    size_t get_pixel_count() const;             // number of pixels
    size_t get_size() const;                    // size in bytes of the pixel array

    // Return a pointer to the tile' storage.
    uint8* get_storage() const;

    // Direct access to a given pixel.
    uint8* pixel(
        const size_t        i) const;
    uint8* pixel(
        const size_t        x,
        const size_t        y) const;

    // Direct access to a given component of a given pixel.
    uint8* component(
        const size_t        i,
        const size_t        c) const;
    uint8* component(
        const size_t        x,
        const size_t        y,
        const size_t        c) const;

    // Structured write access to a given pixel, with automatic pixel format conversion.
    template <typename T>
    void set_pixel(
        const size_t        i,
        const T             components[]);
    template <typename T>
    void set_pixel(
        const size_t        x,
        const size_t        y,
        const T             components[]);
    template <typename Color>
    void set_pixel(
        const size_t        i,
        const Color&        color);
    template <typename Color>
    void set_pixel(
        const size_t        x,
        const size_t        y,
        const Color&        color);

    // Structured write access to a given component of a given pixel.
    template <typename T>
    void set_component(
        const size_t        i,
        const size_t        c,
        const T             value);
    template <typename T>
    void set_component(
        const size_t        x,
        const size_t        y,
        const size_t        c,
        const T             value);

    // Structured read access to a given pixel, with automatic pixel format conversion.
    template <typename T>
    void get_pixel(
        const size_t        i,
        T                   components[]) const;
    template <typename T>
    void get_pixel(
        const size_t        x,
        const size_t        y,
        T                   components[]) const;
    template <typename Color>
    void get_pixel(
        const size_t        i,
        Color&              color) const;
    template <typename Color>
    void get_pixel(
        const size_t        x,
        const size_t        y,
        Color&              color) const;

    // Structured read access to a given component of a given pixel.
    template <typename T>
    T get_component(
        const size_t        i,
        const size_t        c) const;
    template <typename T>
    T get_component(
        const size_t        x,
        const size_t        y,
        const size_t        c) const;

    // Set all pixels to a given color.
    template <typename Color>
    void clear(const Color& color);

    // Copy the contents of another tile of equal dimensions and number of channels
    // (but possibly using a different pixel format).
    void copy(const Tile& rhs);

  protected:
    const size_t            m_width;            // tile width, in pixels
    const size_t            m_height;           // tile height, in pixels
    const size_t            m_channel_count;    // number of channels per pixel
    const PixelFormat       m_pixel_format;     // pixel format
    const size_t            m_pixel_count;      // total number of pixels
    const size_t            m_channel_size;     // size in bytes of one channel
    const size_t            m_pixel_size;       // size in bytes of one pixel
    const size_t            m_array_size;       // size in bytes of the pixel array
    uint8*                  m_pixel_array;      // pixel array
    bool                    m_own_storage;      // does the tile own the memory used for pixel storage?
};


//
// Tile class implementation.
//

inline PixelFormat Tile::get_pixel_format() const
{
    return m_pixel_format;
}

inline size_t Tile::get_width() const
{
    return m_width;
}

inline size_t Tile::get_height() const
{
    return m_height;
}

inline size_t Tile::get_channel_count() const
{
    return m_channel_count;
}

inline size_t Tile::get_pixel_count() const
{
    return m_pixel_count;
}

inline size_t Tile::get_size() const
{
    return m_array_size;
}

inline uint8* Tile::get_storage() const
{
    return m_pixel_array;
}

inline uint8* Tile::pixel(
    const size_t    i) const
{
    assert(i < m_pixel_count);

    const size_t index = i * m_pixel_size;
    assert(index < m_array_size);

    return m_pixel_array + index;
}

inline uint8* Tile::pixel(
    const size_t    x,
    const size_t    y) const
{
    assert(x < m_width);
    assert(y < m_height);

    return pixel(y * m_width + x);
}

inline uint8* Tile::component(
    const size_t    i,
    const size_t    c) const
{
    return pixel(i) + c * m_channel_size;
}

inline uint8* Tile::component(
    const size_t    x,
    const size_t    y,
    const size_t    c) const
{
    return pixel(x, y) + c * m_channel_size;
}

template <typename T>
inline void Tile::set_pixel(
    const size_t    i,
    const T         components[])
{
    Pixel::convert_to_format(
        components,                             // source begin
        components + m_channel_count,           // source end
        1,                                      // source stride
        m_pixel_format,                         // destination format
        pixel(i),                               // destination
        1);                                     // destination stride
}

template <typename T>
inline void Tile::set_pixel(
    const size_t    x,
    const size_t    y,
    const T         components[])
{
    assert(x < m_width);
    assert(y < m_height);

    set_pixel<T>(y * m_width + x, components);
}

template <typename Color>
inline void Tile::set_pixel(
    const size_t    i,
    const Color&    color)
{
    assert(sizeof(Color) == m_channel_count * sizeof(color[0]));

    set_pixel(i, &color[0]);
}

template <typename Color>
inline void Tile::set_pixel(
    const size_t    x,
    const size_t    y,
    const Color&    color)
{
    assert(sizeof(Color) == m_channel_count * sizeof(color[0]));

    set_pixel(x, y, &color[0]);
}

template <typename T>
inline void Tile::set_component(
    const size_t    i,
    const size_t    c,
    const T         value)
{
    Pixel::convert_to_format(
        &value,                                 // source begin
        &value + 1,                             // source end
        1,                                      // source stride
        m_pixel_format,                         // destination format
        component(i, c),                        // destination
        1);                                     // destination stride
}

template <typename T>
inline void Tile::set_component(
    const size_t    x,
    const size_t    y,
    const size_t    c,
    const T         value)
{
    assert(x < m_width);
    assert(y < m_height);

    set_component(y * m_width + x, c, value);
}

template <typename T>
inline void Tile::get_pixel(
    const size_t    i,
    T               components[]) const
{
    const uint8* src = pixel(i);

    Pixel::convert_from_format(
        m_pixel_format,                         // source format
        src,                                    // source begin
        src + m_pixel_size,                     // source end
        1,                                      // source stride
        components,                             // destination
        1);                                     // destination stride
}

template <typename T>
inline void Tile::get_pixel(
    const size_t    x,
    const size_t    y,
    T               components[]) const
{
    assert(x < m_width);
    assert(y < m_height);

    get_pixel<T>(y * m_width + x, components);
}

template <typename Color>
inline void Tile::get_pixel(
    const size_t    i,
    Color&          color) const
{
    assert(sizeof(Color) == m_channel_count * sizeof(color[0]));

    get_pixel(i, &color[0]);
}

template <typename Color>
inline void Tile::get_pixel(
    const size_t    x,
    const size_t    y,
    Color&          color) const
{
    assert(sizeof(Color) == m_channel_count * sizeof(color[0]));

    get_pixel(x, y, &color[0]);
}

template <typename T>
inline T Tile::get_component(
    const size_t    i,
    const size_t    c) const
{
    const uint8* src = component(i, c);

    T value;
    Pixel::convert_from_format(
        m_pixel_format,                         // source format
        src,                                    // source begin
        src + m_channel_size,                   // source end
        1,                                      // source stride
        &value,                                 // destination
        1);                                     // destination stride

    return value;
}

template <typename T>
inline T Tile::get_component(
    const size_t    x,
    const size_t    y,
    const size_t    c) const
{
    assert(x < m_width);
    assert(y < m_height);

    return get_component<T>(y * m_width + x, c);
}

template <typename Color>
inline void Tile::clear(const Color& color)
{
    assert(sizeof(Color) == m_channel_count * sizeof(color[0]));

    // Set first pixel of first row.
    uint8* base = pixel(0, 0);
    Pixel::convert_to_format(
        &color[0],                              // source begin
        &color[0] + m_channel_count,            // source end
        1,                                      // source stride
        m_pixel_format,                         // destination format
        base,                                   // destination
        1);                                     // destination stride

    // Set remaining pixels of first row.
    uint8* dest = base + m_pixel_size;
    for (size_t i = 1; i < m_width; ++i)
    {
        std::memcpy(dest, base, m_pixel_size);
        dest += m_pixel_size;
    }

    // Set remaining rows.
    const size_t row_size = m_width * m_pixel_size;
    for (size_t i = 1; i < m_height; ++i)
    {
        std::memcpy(dest, base, row_size);
        dest += row_size;
    }
}

}       // namespace foundation

#endif  // !APPLESEED_FOUNDATION_IMAGE_TILE_H
