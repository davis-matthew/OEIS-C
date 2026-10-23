/*
 * A173984  a(n) is the denominator of (Zeta(0,2,1/3) - Zeta(0,2,n+1/3)) where
 *          Zeta is the Hurwitz Zeta function.
 * https://oeis.org/A173984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of sum_{k=1}^{n-1} 9/(3*k+1)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We maintain the fraction in reduced form throughout the loop. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) and a(1) are 1 because there are no terms to sum. */
    if (n <= 1)
        return (seq_t)1;

    seq_t num = (seq_t)0;   /* numerator of the running sum */
    seq_t den = (seq_t)1;   /* denominator of the running sum */

    for (idx_t k = (idx_t)1; k < n; ++k) {
        /* term = 9 / (3*k+1)^2  */
        seq_t d       = (seq_t)(3 * k + 1);
        seq_t t_num   = (seq_t)9;
        seq_t t_den   = d * d;

        /* Compute lcm of current denominator and term denominator
           and combine the fractions in a reduced way. */
        seq_t g1     = gcd(den, t_den);
        seq_t lcm    = (den / g1) * t_den;             /* lcm of den and t_den */
        seq_t num1   = num * (t_den / g1);              /* num * (t_den/g1) */
        seq_t num2   = t_num * (den / g1);             /* t_num * (den/g1) */
        seq_t new_num= num1 + num2;
        seq_t new_den= lcm;

        /* Reduce the new fraction */
        seq_t g2     = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return den;   /* denominator of the reduced sum */
}
