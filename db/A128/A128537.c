/*
 * A128537  a(n) = denominator of r(n): r(n) is such that, for every positive
 *          integer n, the continued fraction (of rational terms)
 *          [r(1);r(2),...,r(n)] equals n(n+1)/2, the n-th triangular number.
 * https://oeis.org/A128537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = denominator of the rational term r(n) in the continued fraction
 *  representation of the nth triangular number:
 *  [r(1); r(2),…, r(n)] = n(n+1)/2
 *
 *  The recurrence (valid for n ≥ 4) is:
 *      r(n) = - (2n-1)(2n-3) / [ n(n-2) * r(n-1) ]
 *
 *  The first values are:
 *      r(1) = 1/1          (denominator 1)
 *      r(2) = 1/2          (denominator 2)
 *      r(3) = -10/3        (denominator 3)
 *
 *  The function returns the denominator of r(n) for any n ≥ 1.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Handle the first three values explicitly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* r(1)  = 1/1   -> sign +, num=1, den=1
     * r(2)  = 1/2   -> sign +, num=1, den=2
     * r(3)  = -10/3 -> sign -, num=10, den=3  */
    seq_t num_mag = (seq_t)10;   /* absolute value of the numerator of r(3) */
    seq_t den_mag = (seq_t)3;    /* denominator of r(3)             */
    int  num_sign = -1;          /* -1 for negative, +1 for positive */

    /* Iterate the recurrence from k = 4 up to n. */
    for (idx_t k = 4; k <= n; ++k) {
        /* Compute the coefficient (2k-1)(2k-3). */
        seq_t coeff = (seq_t)(2 * k - 1) * (seq_t)(2 * k - 3);

        /* Compute the denominator part n(n-2). */
        seq_t denom_part = (seq_t)k * (seq_t)(k - 2);

        /* New numerator magnitude and sign:
         *    r(k) = -(coeff) / (denom_part * r(k-1))
         *    Sign becomes -num_sign. */
        seq_t new_num_mag = coeff * den_mag;
        int   new_num_sign = -num_sign;

        /* New denominator magnitude:
         *    denom = denom_part * num_mag. */
        seq_t new_den_mag = denom_part * num_mag;

        /* Reduce the fraction by the GCD of the magnitudes. */
        seq_t g = seq_gcd(new_num_mag, new_den_mag);
        new_num_mag /= g;
        new_den_mag /= g;

        /* Prepare for next iteration. */
        num_mag  = new_num_mag;
        den_mag  = new_den_mag;
        num_sign = new_num_sign;
    }

    return den_mag;          /* denominator of r(n) (always positive) */
}
