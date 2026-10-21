/*
 * A061484  Numerator of 1/3 + 3/5 + 5/7 + ... + (2n - 1)/(2n + 1).
 * https://oeis.org/A061484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the numerator of the reduced fraction S(n) = Σ_{k=1}^n
 *            (2k-1)/(2k+1) (with a(0)=0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* The empty sum is 0 */

    seq_t num = 0;                 /* numerator of the partial sum */
    seq_t den = 1;                 /* denominator of the partial sum (reduced) */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t term_num = (seq_t)2 * k - 1;   /* numerator of the k‑th term   */
        seq_t term_den = (seq_t)2 * k + 1;   /* denominator of the k‑th term */

        seq_t g   = gcd(den, term_den);      /* gcd of current denominator
                                                 and term denominator */
        seq_t den_div   = den   / g;          /* den / g                     */
        seq_t term_den_div = term_den / g;   /* term_den / g                */

        /* Combine the fractions:
           num/den  +  term_num/term_den
           = (num * (term_den/g) + (den/g) * term_num) / ( (den/g) * term_den )
        */
        seq_t new_num = num * term_den_div + den_div * term_num;
        seq_t new_den = den_div * term_den;      /* (den/g) * term_den */

        /* Reduce the resulting fraction */
        seq_t g2       = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;  /* the numerator of the reduced sum */
}
