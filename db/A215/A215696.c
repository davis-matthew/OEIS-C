/*
 * A215696  a(n)=smallest positive k>n+2 such that k*n+1 is a square.
 * https://oeis.org/A215696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > n+2 such that k*n + 1 is a perfect square
 * Range:     u8 n <= 4, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 974 (2026-11-01)
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
    /* Search k from n+3 upwards until k*n+1 is a square. */
    for (idx_t k = n + 3; ; ++k) {
        seq_t t = (seq_t)k * (seq_t)n + 1;
        seq_t s = floor_sqrt(t);
        if (s * s == t)   /* perfect square found */
            return (seq_t)k;
    }
}
