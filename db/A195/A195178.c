/*
 * A195178  a(n) = 4*n - floor(2*n*sqrt(2)).
 * https://oeis.org/A195178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n - floor(2*n*sqrt(2))
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9800 (2026-11-01)
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
    seq_t n_seq = (seq_t)n;
    seq_t m = n_seq * n_seq;   // n²
    m *= (seq_t)8;             // 8*n² = (2*n)² * 2
    seq_t s = floor_sqrt(m);   // floor( sqrt(8*n²) ) = floor(2*n*sqrt(2) )
    return (seq_t)4 * n_seq - s;
}
