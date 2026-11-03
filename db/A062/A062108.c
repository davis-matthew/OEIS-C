/*
 * A062108  a(n) = floor(n^(3/4)).
 * https://oeis.org/A062108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(3/4))
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 92 terms through n = 990 (2026-11-01)
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
    seq_t nseq = (seq_t)n;
    seq_t n3   = nseq * nseq * nseq;   // n^3
    seq_t r    = floor_sqrt(n3);       // floor((n^3)^(1/2)) = floor(n^(3/2))
    seq_t res  = floor_sqrt(r);        // floor((n^(3/2))^(1/2)) = floor(n^(3/4))
    return res;
}
