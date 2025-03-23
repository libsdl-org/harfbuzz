/*
 * Copyright © 2022 Red Hat, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Author: Matthias Clasen
 */


#include "hb-test.h"

#include "hb-directwrite.h"

/* Declare object creator for dynamic support of DWRITE */
typedef HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

IDWriteFontFace *
get_dwfontface (const char *font_path)
{
  auto *face = hb_test_open_font_file (font_path);

  auto *dw_font_face = hb_directwrite_face_get_dw_font_face (face);

  dw_font_face->AddRef ();

  hb_face_destroy (face);

  return dw_font_face;
}

static void
test_native_directwrite_basic (void)
{
  IDWriteFontFace *dwfontface;
  hb_font_t *font;
  IDWriteFontFace *dwfontface2;

  dwfontface = get_dwfontface ("fonts/Roboto-Regular.abc.ttf");
  g_assert_nonnull (dwfontface);

  font = hb_directwrite_font_create (dwfontface);

  dwfontface2 = hb_directwrite_font_get_dw_font_face (font);

  g_assert_true (dwfontface2 == dwfontface);

  hb_font_destroy (font);

  dwfontface->Release ();
}


static void
test_native_directwrite_variations (void)
{
  IDWriteFontFace *dwfontface;
  hb_font_t *font;
  unsigned int length;

  dwfontface = get_dwfontface ("fonts/AdobeVFPrototype.abc.ttf");
  g_assert_nonnull (dwfontface);

  font = hb_directwrite_font_create (dwfontface);
  hb_font_get_var_coords_normalized(font, &length);
  g_assert_cmpuint (length, !=, 0);

  hb_font_destroy (font);

  dwfontface->Release ();
}


int
main (int argc, char **argv)
{
  hb_test_init (&argc, &argv);

  hb_test_add (test_native_directwrite_basic);
  hb_test_add (test_native_directwrite_variations);

  return hb_test_run ();
}
