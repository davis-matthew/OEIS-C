/*
 * A108967  This finite sequence is obtained by replacing the letters you are
 *          reading now with their rank in the alphabet.
 * https://oeis.org/A108967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word, dumb, less
 * Formula:   a(n) = rank in alphabet of nth letter (ignoring non‑letters) in
 *            the sentence "This finite sequence is obtained by replacing the
 *            letters you are reading now with their rank in the alphabet."
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>

/* External linkage */
seq_t a(idx_t n)
{
    /* The source sentence used by the OEIS entry. */
    static const char *const sentence =
        "This finite sequence is obtained by replacing the letters you are "
        "reading now with their rank in the alphabet.";
    idx_t pos = 1;               /* current letter position (1‑based) */
    for (const char *p = sentence; *p; ++p)
    {
        char c = *p;
        /* Keep only alphabetic characters. */
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            if (pos == n)
            {
                /* Convert to lower case to get a=1…z=26. */
                char lower = (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
                return (seq_t)(lower - 'a' + 1);
            }
            ++pos;
        }
    }
    /* n is beyond the end of the sentence → return 0 (undefined). */
    return 0;
}
