/*
 * A024028  a(n) = 3^n - n^5.
 * https://oeis.org/A024028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3^n - n^5
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 188 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow3 = ipow_idx((seq_t)3, n);
    seq_t pow5 = ipow_idx((seq_t)n, (idx_t)5);
    return pow3 - pow5;
}
