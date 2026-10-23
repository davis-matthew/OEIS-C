/*
 * A100513  Denominator of Sum_{k=0..n} 1/C(2*n,2*k).
 * https://oeis.org/A100513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator( Σ_{k=0..n} 1 / C(2n, 2k) )
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd_seq.h"

seq_t a(idx_t n)
{
    /* current sum num/den = Σ 1/binom
       binom = C(2n, 2k) for current k                */
    seq_t num = 0;          /* numerator          */
    seq_t den = 1;          /* denominator        */
    seq_t binom = 1;        /* C(2n,0)            */
    seq_t N = 2 * (seq_t)n; /* 2n as seq_t        */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* add 1/binom to num/den */
        /* Reduce fraction before multiplying to keep values small */
        seq_t g = gcd_seq(den, binom);
        seq_t den1 = den / g;
        seq_t bin1 = binom / g;

        /* new numerator and denominator before reduction */
        num = num * bin1 + den1;
        den = den1 * binom;   /* binom = g * bin1 */

        /* reduce the fraction */
        seq_t g2 = gcd_seq(num, den);
        if (g2 > 1)
        {
            num /= g2;
            den /= g2;
        }

        /* advance binom to C(2n, 2(k+1)) if needed */
        if (k < n)
        {
            /* r = 2k as seq_t */
            seq_t r = 2 * (seq_t)k;
            /* binom_next = binom * (N - r) * (N - r - 1) / ((r + 1) * (r + 2)) */
            seq_t numerator   = binom * (N - r) * (N - r - 1);
            seq_t denominator = (r + 1) * (r + 2);
            binom = numerator / denominator;   /* exact division */
        }
    }

    /* at this point num/den is the reduced sum;
       the requested value is the denominator */
    return den;
}
