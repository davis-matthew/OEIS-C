/*
 * A024118  a(n) = 10^n - n^4.
 * https://oeis.org/A024118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - n^4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 292 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow10 = ipow_idx((seq_t)10, n);
    seq_t x    = (seq_t)n;
    seq_t x2   = x * x;
    seq_t x4   = x2 * x2;
    return pow10 - x4;
}
