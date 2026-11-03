/*
 * A214053  Least m>0 such that floor(n*r)+m and n-m have a common divisor >
 *          1, where r = (1+sqrt(5))/2, the golden ratio.
 * https://oeis.org/A214053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m>0 such that floor(n*r)+m and n-m have a common
 *            divisor > 1, where r = (1+sqrt(5))/2 (the golden ratio)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 98 terms through n = 927 (2026-11-01)
 * Helpers:   gcd, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
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
    /* Compute floor(n*phi) using integer arithmetic:
       floor(n*phi) = floor((n + floor(sqrt(5)*n)) / 2)
       Here floor(sqrt(5)*n) = floor_sqrt(5*n*n). */
    seq_t n_seq = (seq_t)n;
    seq_t sq = floor_sqrt((seq_t)5 * n_seq * n_seq);
    seq_t floor_nphi = (n_seq + sq) / 2;

    /* Search for the smallest m>0 such that gcd(floor_nphi + m, n - m) > 1. */
    for (idx_t m = 1; m <= n; ++m)
    {
        seq_t g = gcd(floor_nphi + (seq_t)m, n_seq - (seq_t)m);
        if (g > (seq_t)1)
            return (seq_t)m;
    }

    /* In theory a solution always exists (m=n works); the following is a safety fallback. */
    return (seq_t)n;
}
