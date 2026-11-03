/*
 * A036102  Centered cube numbers: (n+1)^24 + n^24.
 * https://oeis.org/A036102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^24 + n^24
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 5, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 9754 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t b1 = (seq_t)n + (seq_t)1;
    seq_t b0 = (seq_t)n;

    seq_t p1 = ipow_idx(b1, 24);
    seq_t p0 = ipow_idx(b0, 24);

    return p1 + p0;
}
