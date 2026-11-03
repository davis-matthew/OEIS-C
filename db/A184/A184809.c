/*
 * A184809  a(n) = n + floor(sqrt(3/2)*n).
 * https://oeis.org/A184809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(3/2) * n)
 * Range:     u8 n <= 6, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 971 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Beatty sequence for 1 + sqrt(3/2).  No floating point used. */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* cast n once to avoid truncation during multiplication */
    seq_t nn  = n;
    seq_t n_sq = nn * nn;          /* n² */
    seq_t tmp   = n_sq * 3;         /* 3·n² */
    seq_t div2  = tmp / 2;          /* ⌊3·n²/2⌋ */
    seq_t r_floor = floor_sqrt(div2); /* ⌊√(3·n²/2)⌋ = ⌊√(3/2)·n⌋ */
    return nn + r_floor;
}
