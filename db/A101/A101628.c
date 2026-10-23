/*
 * A101628  Denominator of partial sums of a certain series.
 * https://oeis.org/A101628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of 8*sum_{k=1}^{n} 1/((3*k-1)*k*(3*k+1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

seq_t
a(idx_t n)
{
    seq_t numer = 0;   /* current numerator of the partial sum */
    seq_t denom = 1;   /* current denominator of the partial sum */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;

        /* denominator of the k‑th term: (3k-1)*k*(3k+1) */
        seq_t den = (3 * kk - 1) * kk * (3 * kk + 1);

        /* reduce before adding to keep numbers small when possible */
        seq_t g = gcd_seq(denom, den);
        seq_t den_over_g = den / g;
        seq_t denom_over_g = denom / g;

        /* new numerator and denominator after adding 8/den */
        seq_t new_num = numer * den_over_g + (seq_t)8 * denom_over_g;
        seq_t new_den = denom_over_g * den;  /* = denom/g * den */

        /* reduce the new fraction */
        seq_t g2 = gcd_seq(new_num, new_den);
        numer = new_num / g2;
        denom = new_den / g2;
    }

    return denom;
}
