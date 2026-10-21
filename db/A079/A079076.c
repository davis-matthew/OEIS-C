/*
 * A079076  Numerator of Sum_{1<k<n, n mod k > 0} n/k.
 * https://oeis.org/A079076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of ∑_{k=2}^{n-1, n mod k ≠ 0} n/k
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The running fraction is kept in lowest terms after each addition. */

#include "../utils/gcd.h"

/* ------------------------------------------------------------------ */
/* Sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* For n = 1 or 2 the sum is empty, so the numerator is 0.      */
    if (n < 3)
        return 0;

    seq_t num = 0;                     /* running numerator          */
    seq_t den = 1;                     /* running denominator        */

    for (idx_t k = 2; k < n; ++k)
    {
        if (n % k == 0)               /* skip divisors of n – term is zero   */
            continue;

        seq_t k_s  = (seq_t)k;        /* avoid implicit promotion to idx_t  */

        /* Add the fraction n/k to num/den.                         */
        seq_t new_num = num * k_s + (seq_t)n * den;
        seq_t new_den = den * k_s;

        /* Reduce the fraction to lowest terms.                     */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* The numerator of the reduced sum is the requested value.      */
    return num;
}
