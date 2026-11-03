/*
 * A024113  a(n) = 9^n-n^12.
 * https://oeis.org/A024113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 9^n - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 19, i128 n <= 40,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 300 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow9 = ipow_idx((seq_t)9, n);
    seq_t pown = ipow_idx((seq_t)n, (idx_t)12);
    return pow9 - pown;
}
