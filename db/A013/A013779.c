/*
 * A013779  a(n) = 3^(4*n + 3).
 * https://oeis.org/A013779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(4*n + 3)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 196 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* exponent = 4*n + 3 (fits in idx_t for all practical n) */
    idx_t exp = 4 * n + 3;
    /* compute 3^exponent using the generic power routine */
    return ipow_idx((seq_t)3, exp);
}
