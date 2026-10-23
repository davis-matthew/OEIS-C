/*
 * A017811  Binomial coefficients C(95,n).
 * https://oeis.org/A017811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial coefficient C(95, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 18, u128 n <= 95,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle indices beyond the row length */
    if (n > 95)
        return 0;

    /* Use symmetry to minimize the number of iterations */
    idx_t k = n;
    if (k > 95 - k)
        k = 95 - k;

    seq_t res = 1;

    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(95 - k + i);   /* numerator */
        seq_t den = (seq_t)i;              /* denominator */

        /* Simplify the fraction before multiplication to keep numbers small */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Further simplify by cancelling factors from the current result */
        g = gcd(res, den);
        res /= g;
        den /= g;

        /* After the above cancellations, den divides res exactly */
        if (den != 1)
            res /= den;

        /* Multiply by the simplified numerator */
        res *= num;
    }

    return res;
}
