/*
 * A025465  Number of partitions of n into 3 distinct nonnegative cubes.
 * https://oeis.org/A025465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17073, u16 n <= 17073, u32 n <= 17073, u64 n <= 17073,
 *            u128 n <= 17073, big any n
 * Validated: standard, pass, 124 terms through n = 9574 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of solutions to n = x^3 + y^3 + z^3 with
 * x, y, z integers satisfying x > y > z >= 0
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
    idx_t maxZ = int_cube_root(n);
    for (idx_t z = 0; z <= maxZ; ++z)
    {
        idx_t z3 = z * z * z;
        if (z3 > n) break;
        idx_t rem1 = n - z3;

        idx_t maxY = int_cube_root(rem1);
        for (idx_t y = z + 1; y <= maxY; ++y)
        {
            idx_t y3 = y * y * y;
            if (y3 > rem1) break;
            idx_t rem2 = rem1 - y3;

            /* x must be greater than y, so x^3 > y^3 */
            if (rem2 <= y3) continue;

            idx_t x = int_cube_root(rem2);
            if (x > y && x * x * x == rem2)
            {
                ++count;
            }
        }
    }
    return count;
}
