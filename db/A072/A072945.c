/*
 * A072945  Number of pairs (x,y) with x,y >= 0 such that x^3+y^2+x*y <= n.
 * https://oeis.org/A072945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (x,y) ≥ 0 with x³ + y² + x·y ≤ n
 * Range:     u8 n <= 15, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
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

seq_t a(idx_t n)
{
    idx_t max_x = int_cube_root(n);
    seq_t total = 0;
    for (idx_t x = 0; x <= max_x; ++x)
    {
        seq_t sub   = (seq_t)n - (seq_t)x * (seq_t)x * (seq_t)x;
        seq_t disc  = (seq_t)x * (seq_t)x + (seq_t)4 * sub;
        seq_t sqrt_disc = floor_sqrt(disc);
        seq_t y_max = (sqrt_disc - (seq_t)x) / 2;
        total += y_max + 1;
    }
    return total;
}
