/*
 * A132296  Sum of the noncube numbers less than or equal to n.
 * https://oeis.org/A132296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 - (r*(r+1)/2)^2, where r = floor(n^(1/3))
 * Range:     u8 n <= 22, u16 n <= 363, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 997 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

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
    /* r is the largest integer with r^3 <= n */
    idx_t r = int_cube_root(n);

    /* sum of all numbers from 1 to n */
    seq_t sum_n = (seq_t)n * ((seq_t)n + 1) / 2;

    /* sum of cubes up to r^3: (1+2+...+r)^2 */
    seq_t sum_cubes = (seq_t)r * ((seq_t)r + 1) / 2;

    return sum_n - sum_cubes * sum_cubes;
}
