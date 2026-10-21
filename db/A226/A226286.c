/*
 * A226286  [n/2]!*[(n+1)/2]!*C([n/2],5)*C([(n+1)/2],5).
 * https://oeis.org/A226286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2)! * floor((n+1)/2)! * C(floor(n/2),5) *
 *            C(floor((n+1)/2),5)
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* floor divisions */
    idx_t k   = n / 2;          /* floor(n/2)   */
    idx_t k1  = (n + 1) / 2;    /* floor((n+1)/2) */

    /* If either binomial coefficient is zero, return zero immediately */
    if (k < 5 || k1 < 5)
        return 0;

    /* factorials */
    seq_t f1 = factorial(k);
    seq_t f2 = factorial(k1);

    /* binomial coefficients */
    seq_t c1 = binom(k, 5);
    seq_t c2 = binom(k1, 5);

    /* product of all factors */
    seq_t result = f1 * f2;
    result *= c1;
    result *= c2;

    return result;
}
