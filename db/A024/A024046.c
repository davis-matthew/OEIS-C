/*
 * A024046  a(n) = 4^n - n^10.
 * https://oeis.org/A024046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 490 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow4   = ipow_idx((seq_t)4, n);
    seq_t pown10 = ipow_idx((seq_t)n, (idx_t)10);
    return pow4 - pown10;
}
