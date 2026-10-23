/*
 * A036506  Number of labeled 4-trees with n nodes.
 * https://oeis.org/A036506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,4) * (4*n-15)^(n-6) (for n >= 6)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle small n explicitly */
    if (n <= 3)
        return (seq_t)0;
    if (n == 4 || n == 5)
        return (seq_t)1;

    /* Binary representation of n in seq_t for further calculations */
    const seq_t nn = (seq_t)n;

    /* Compute binomial coefficient C(n,4) = n*(n-1)*(n-2)*(n-3)/24 */
    seq_t binom = nn * (nn - 1) * (nn - 2) * (nn - 3) / (seq_t)24;

    /* Base and exponent for the power term */
    seq_t base = (seq_t)(4 * n - 15);
    seq_t exp  = (seq_t)(n - 6);

    /* Raise base to the power exp */
    seq_t pow = big_pow(base, exp);

    /* Final result */
    return binom * pow;
}
