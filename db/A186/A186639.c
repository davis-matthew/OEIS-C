/*
 * A186639  a(n) = n!/2-(-2)^(n-2)*(n-2).
 * https://oeis.org/A186639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!/2 - (-2)^(n-2)*(n-2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 25,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return 1;
    if (n == 1) return 0;

    /* n >= 2 */
    seq_t fact = factorial(n);
    fact /= 2;

    idx_t exp = n - 2;
    seq_t pow2_val = pow2(exp);
    seq_t term = pow2_val * (seq_t)exp;           /* 2^(n-2)*(n-2) */

    if (exp & 1) {                                /* odd exponent */
        fact += term;          /* subtract a negative value => add */
    } else {                                /* even exponent */
        fact -= term;          /* subtract a positive value */
    }

    return fact;
}
