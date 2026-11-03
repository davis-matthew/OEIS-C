/*
 * A024097  a(n) = 8^n - n^9.
 * https://oeis.org/A024097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 8^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 21, i128 n <= 42,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 138 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow8   = ipow_idx((seq_t)8, n);
    seq_t pown9  = ipow_idx((seq_t)n, (idx_t)9);
    return pow8 - pown9;
}
