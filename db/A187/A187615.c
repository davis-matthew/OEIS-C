/*
 * A187615  Expansion of f(-x^17, -x^19) + x^4 * f(-x, -x^35) in powers of x
 *          where f(,) is Ramanujan's general theta function.
 * https://oeis.org/A187615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum over k ∈ ℤ of (−1)^k * ( [n = 18k^2 + 17k + 4] + [n =
 *            18k^2 + k] )
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* Determine a conservative upper bound for |k| such that 18*k^2 <= n + 100 */
    int64_t bound = 0;
    __int128 limit = (__int128)n + 100;
    while ((__int128)18 * (bound + 1) * (bound + 1) <= limit) {
        ++bound;
    }
    bound += 5;                      /* small safety margin */

    for (int64_t k = -bound; k <= bound; ++k) {
        /* term 2: exponent = 18k^2 + k */
        __int128 e2 = (__int128)18 * k * k + k;
        if (e2 == (__int128)n) {
            sum += ((k & 1) ? -1 : 1);
        }

        /* term 1: exponent = 18k^2 + 17k + 4 */
        __int128 e1 = (__int128)18 * k * k + (__int128)17 * k + 4;
        if (e1 == (__int128)n) {
            sum += ((k & 1) ? -1 : 1);
        }
    }

    return sum;
}
