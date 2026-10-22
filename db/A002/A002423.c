/*
 * A002423  Expansion of (1-4*x)^(7/2).
 * https://oeis.org/A002423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = 2*(2*n-9)*a(n-1)/n, a(0) = 1 (D-finite recurrence n*a(n)
 *            + 2*(9-2*n)*a(n-1) = 0)
 * Range:     i8 n <= 2, i16 n <= 14, i32 n <= 24, i64 n <= 42, i128 n <= 76,
 *            big any n
 * Helpers:   a002423_gcd_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002423_gcd_u.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; k++) {
        long long m = 2 * (2 * (long long)k - 9);
        uint64_t am = (uint64_t)(m < 0 ? -m : m);
        uint64_t g = a002423_gcd_u((uint64_t)k, am);
        long long q = (long long)((uint64_t)k / g);
        long long c = m / (long long)g;
        /* k/g is coprime to m/g, so it divides result exactly */
        result = result / q;
        result = result * c;
    }
    return result;
}
