/*
 * A024059  a(n) = 5^n - n^10.
 * https://oeis.org/A024059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5^n - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 460 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* compute 5^n */
    seq_t pow5 = ipow_idx((seq_t)5, n);

    /* compute n^10 */
    seq_t n10 = ipow_idx((seq_t)n, (int)10);

    return pow5 - n10;
}
