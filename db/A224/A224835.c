/*
 * A224835  Sum of the cubes of the number of divisors function for those
 *          divisors of n that are less than or equal to the cube root of n.
 * https://oeis.org/A224835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{d|n,\, d \le n^{1/3}} \tau(d)^3
 * Range:     u8 n <= 1079, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9534 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/num_divisors.h"

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
    seq_t sum = 0;
    idx_t cube = int_cube_root(n);
    for (idx_t d = 1; d <= cube; ++d) {
        if (n % d == 0) {
            seq_t t = num_divisors(d);
            sum += t * t * t;
        }
    }
    return sum;
}
