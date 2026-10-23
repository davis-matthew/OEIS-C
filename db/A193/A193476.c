/*
 * A193476  The denominators of the Bernoulli secant numbers at odd indices.
 * https://oeis.org/A193476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(2n+1) * (2^(2n+1)-1) / gcd(2n+1 , 2^(2n+1)-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* odd integer m = 2n + 1 */
    idx_t m = 2 * n + 1;

    /* P = 2^(2n+1) */
    seq_t P = pow2(m);

    /* Q = 2^(2n+1) - 1 */
    seq_t Q = P - 1;

    /* Numerator = P * Q */
    seq_t num = P * Q;

    /* Euclidean algorithm for gcd(m , Q) */
    seq_t x = (seq_t)m;
    seq_t y = Q;
    while (y != 0) {
        seq_t r = x % y;
        x = y;
        y = r;
    }

    /* Return the reduced denominator */
    return num / x;
}
