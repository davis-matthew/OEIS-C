/*
 * A077113  Number of nonnegative integer cubes <= n^2.
 * https://oeis.org/A077113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(2/3)) + 1
 * Range:     u8 n <= 7, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9872 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the least m such that m^3 > n^2. */
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t r  = floor_cube_root(sq);
    return r + 1;
}
