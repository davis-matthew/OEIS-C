/*
 * A024058  a(n) = 5^n - n^9.
 * https://oeis.org/A024058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 487 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow5 = ipow_idx((seq_t)5, n);        /* 5^n */
    seq_t pow9 = ipow_idx((seq_t)n, 9);        /* n^9 */
    return pow5 - pow9;
}
