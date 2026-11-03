/*
 * A025456  Number of partitions of n into 3 positive cubes.
 * https://oeis.org/A025456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 positive cubes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 124 terms through n = 9845 (2026-11-01)
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
    seq_t count = 0;
    idx_t max_a = int_cube_root(n);   /* largest i with i^3 <= n */

    for (idx_t a = 1; a <= max_a; ++a) {
        idx_t a3 = a * a * a;

        for (idx_t b = a; b <= max_a; ++b) {
            idx_t b3 = b * b * b;
            idx_t sum_ab = a3 + b3;
            if (sum_ab > n) break;   /* further b only increase sum */

            idx_t remaining = n - sum_ab;
            idx_t c = int_cube_root(remaining);

            if (c < b) continue;    /* enforce a <= b <= c */

            if (c * c * c == remaining) {
                ++count;
            }
        }
    }

    return count;
}
