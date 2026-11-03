/*
 * A023962  First digit after decimal point of cube root of n.
 * https://oeis.org/A023962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10 * (∛n - floor(∛n))) = floor_cube_root(1000 * n)
 *            – 10 * floor_cube_root(n)
 * Range:     u8 none, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 118 terms through n = 958 (2026-11-01)
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
    /* compute integer cube root of n */
    seq_t r = floor_cube_root((seq_t)n);

    /* compute integer cube root of 1000*n */
    seq_t r1000 = floor_cube_root((seq_t)1000 * (seq_t)n);

    /* first decimal digit = r1000 - 10*r */
    return r1000 - (seq_t)10 * r;
}
