/*
 * A190344  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sqrt(5/2), t=sqrt(2/5).
 * https://oeis.org/A190344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [n*sqrt(5/2)] + [n*sqrt(2/5)]
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9807 (2026-11-01)
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

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t n_sq = N * N;
    seq_t temp = n_sq * (seq_t)10;
    seq_t t = floor_sqrt(temp);
    return N + t / (seq_t)2 + t / (seq_t)5;
}
