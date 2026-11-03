/*
 * A214206  a(n) = largest m such that m*(m+1)/2 <= 14*n.
 * https://oeis.org/A214206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest m such that m*(m+1)/2 <= 14*n
 * Range:     u8 n <= 0, u16 n <= 96, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 109 terms through n = 991 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    seq_t C = (seq_t)n * 14;          /* 14*n */
    seq_t t = C * 8 + 1;              /* 1 + 8*C */
    seq_t s = floor_sqrt(t);          /* floor(sqrt(1 + 8*14*n)) */
    seq_t m = (s - 1) / 2;            /* floor((s-1)/2) gives desired m */
    return m;
}
