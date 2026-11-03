/*
 * A118558  a(n) = (2^n-1)^4 - 2.
 * https://oeis.org/A118558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (2^n - 1)^4 - 2
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t p = ipow_idx((seq_t)2, n);

    /* 2^n - 1 */
    seq_t v = p - 1;

    /* (2^n - 1)^2 */
    seq_t v2 = v * v;

    /* (2^n - 1)^4 = ((2^n - 1)^2)^2 */
    seq_t v4 = v2 * v2;

    /* result: (2^n - 1)^4 - 2 */
    return v4 - 2;
}
