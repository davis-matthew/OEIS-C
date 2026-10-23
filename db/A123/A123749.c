/*
 * A123749  Numerators of partial sums of a series for 3/sqrt(5) =
 *          (3/5)*sqrt(5).
 * https://oeis.org/A123749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of sum_{k=0..n} binomial(2*k,k)/9^k in lowest
 *            terms
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t s = 0;          /* aggregated numerator with common denominator 9^n   */
    seq_t binom = 1;      /* C(0,0) */
    seq_t pow9 = 1;       /* 9^k, accumulates to 9^n                            */

    for (idx_t k = 0; k <= n; ++k) {
        /* accumulate numerator: s = s*9 + binom   */
        s = s * 9 + binom;

        if (k < n) {
            /* update binomial to C(2(k+1),k+1) = C(2k,k) * (4k+2)/(k+1) */
            seq_t mult = (seq_t)(4 * k + 2);
            binom = (binom * mult) / (k + 1);

            /* advance common denominator to 9^(k+1) */
            pow9 *= 9;
        }
    }

    /* reduce the fraction s / 9^n and return the numerator */
    seq_t g = gcd(s, pow9);
    return s / g;
}
