/*
 * A025464  Number of partitions of n into 2 distinct nonnegative cubes.
 * https://oeis.org/A025464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into two distinct non‑negative cubes
 * Range:     u8 n <= 63, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Validated: partial, pass, 108 terms through n = 107 (2026-11-01)
 * Helpers:   ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

/* integer cube root: largest k with k³ ≤ x */
static idx_t int_cube_root(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

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
    seq_t sn = n;
    idx_t max_a = int_cube_root(n);
    for (idx_t a = 0; a <= max_a; ++a)
    {
        seq_t aa = a;
        seq_t acube = aa * aa * aa;
        if (acube >= sn)
            break;
        seq_t rem = sn - acube;
        seq_t b = floor_cube_root(rem);
        if (b <= aa)
            continue;
        seq_t bcube = ipow_idx(b, 3);
        if (bcube == rem)
            count++;
    }
    return count;
}
