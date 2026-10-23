/*
 * A029651  Central elements of the (1,2)-Pascal triangle A029635.
 * https://oeis.org/A029651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * binomial(2n-1, n) for n>0, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute binomial(2n-1, n) multiplicatively.
       Using gcd reductions to avoid overflow. */
    seq_t res = 1;
    idx_t n_minus_1 = n - 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = (seq_t)(n_minus_1 + i);   /* (n-1)+i = n-1+i */
        seq_t den = (seq_t)i;

        /* Reduce denominator with current result. */
        seq_t g = gcd(res, den);
        res /= g;
        den /= g;

        /* Reduce remaining denominator with numerator. */
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* After reductions den should be 1.  Multiply. */
        res *= num;
        /* If den != 1, it would indicate an error in the algorithm
           but that cannot happen for binomial coefficients. */
    }

    return res * (seq_t)3;
}
