/*
 * A017814  Binomial coefficients C(98,n).
 * https://oeis.org/A017814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(98, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 17, u128 n <= 98,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices beyond the row length */
    if (n > 98)
        return (seq_t)0;

    /* Use symmetry: C(98, n) == C(98, 98 - n) */
    idx_t k = n;
    if (k > 98 - k)
        k = 98 - k;

    /* Trivial cases */
    if (k == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;

    for (idx_t i = 1; i <= k; ++i)
    {
        /* Current numerator and denominator terms */
        seq_t num = (seq_t)(98 - k + i);   /* 98 - (k - i) */
        seq_t den = (seq_t)i;

        /* Reduce num/den by their greatest common divisor first */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Reduce the remaining denominator against the current result */
        g = gcd(res, den);
        res /= g;
        den /= g;

        /* Multiply by the numerator and divide by any leftover denominator */
        res *= num;
        if (den != (seq_t)1)
            res /= den;   /* division is guaranteed to be exact */
    }

    return res;
}
