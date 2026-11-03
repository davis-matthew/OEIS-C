/*
 * A189405  n+[ns/r]+[nt/r]; r=1, s=-1+sqrt(3), t=1+sqrt(3).
 * https://oeis.org/A189405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 2 * floor(n * sqrt(3)) Using floor_sqrt from the
 *            utils library: floor(n * sqrt(3)) = floor( sqrt(3 * n^2) )
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-01)
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
    // Convert n to seq_t for arithmetic
    seq_t nn = (seq_t)n;

    // Compute floor_sqrt(3 * n^2)
    seq_t m = nn * nn;      // n^2
    m = m * (seq_t)3;       // 3 * n^2
    seq_t sq = floor_sqrt(m); // floor(n * sqrt(3))

    // a(n) = n + 2 * floor(n * sqrt(3))
    seq_t result = (seq_t)n + sq * (seq_t)2;
    return result;
}
