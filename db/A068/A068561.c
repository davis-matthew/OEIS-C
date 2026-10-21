/*
 * A068561  Numerators of coefficients in (1+x)^(1/3)-(1-x)^(1/3) power
 *          series.
 * https://oeis.org/A068561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068561:
   a(n) = numerator of the coefficient of x^(2n-1) in
   (1+x)^(1/3) - (1-x)^(1/3)
   (only odd powers appear; the coefficient is 2*binom(1/3,2n-1)).
*/

#include <stdint.h>

/* Compute gcd of two seq_t values.  Works for built‑in and arbitrary‑precision types. */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* a(1) = 2, so handle n==1 directly.  For n>=1 we iterate. */
    if (n <= 1) return (seq_t)2;

    seq_t num = (seq_t)2;  /* current numerator   a_i   */
    seq_t den = (seq_t)3;  /* current denominator b_i   */

    for (idx_t i = 1; i < n; ++i) {
        /* p = (6i-4)*(6i-1)   ,   q = 9*(2i)*(2i+1)   */
        seq_t p = (seq_t)(6*i-4) * (seq_t)(6*i-1);
        seq_t q = (seq_t)9 * (seq_t)(2*i) * (seq_t)(2*i+1);

        /* cancel common factors between num and q   */
        seq_t g1 = gcd_seq_t(num, q);
        seq_t num1 = num / g1;
        seq_t q1   = q   / g1;

        /* cancel common factors between p and den   */
        seq_t g2 = gcd_seq_t(p, den);
        seq_t p1   = p / g2;
        seq_t den1 = den / g2;

        /* cancel remaining common factors between p1 and q1   */
        seq_t g3 = gcd_seq_t(p1, q1);
        seq_t p2 = p1 / g3;
        seq_t q2 = q1 / g3;

        /* multiply while keeping intermediates in 128‑bit type to avoid overflow
           (the intermediate product fits into 128 bits because p2 and q2 are <= 4*10^9). */
        unsigned __int128 tmp_num = (unsigned __int128)num1 * (unsigned __int128)p2;
        unsigned __int128 tmp_den = (unsigned __int128)den1 * (unsigned __int128)q2;

        num = (seq_t)tmp_num;
        den = (seq_t)tmp_den;
    }

    return num;   /* numerator in reduced form */
}
