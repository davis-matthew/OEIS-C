/*
 * A025455  a(n) is the number of partitions of n into 2 positive cubes.
 * https://oeis.org/A025455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (x,y) of positive integers with x^3 + y^3
 *            = n, x ≥ y > 0
 * Range:     u8 n <= 64, u16 n <= 9923, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 124 terms through n = 9923 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    seq_t count = 0;

    /* n = 0 or 1 cannot be written as a sum of two positive cubes */
    if (n < 2)
        return 0;

    /* y must satisfy y^3 ≤ n/2, so y ≤ floor(cbrt(n/2)) */
    idx_t half   = n >> 1;                       /* integer division by 2 */
    idx_t max_y  = (idx_t)floor_cube_root((seq_t)half);

    for (idx_t y = 1; y <= max_y; ++y) {
        seq_t y3 = (seq_t)y * y * y;
        if (y3 > (seq_t)n)
            break;                               /* safety guard */

        seq_t remaining = (seq_t)n - y3;
        seq_t rseq      = floor_cube_root(remaining);
        idx_t r         = (idx_t)rseq;

        if ((seq_t)r * r * r == remaining && r >= y)
            ++count;
    }

    return count;
}
