/*
 * A146753  a(n) = denominator((1/2)*(1 + Product_{k=0..n-1} 2*(1 + 3*k)/(5 +
 *          6*k))).
 * https://oeis.org/A146753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((1/2)*(1 + Product_{k=0..n-1}
 *            2*(1+3*k)/(5+6*k)))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 1;   /* numerator of the product P(n) */
    seq_t den = 1;   /* denominator of the product P(n) */

    idx_t k;
    for (k = 0; k < n; ++k) {
        /* compute factors 2*(1+3*k) / (5+6*k) and multiply */
        seq_t a_factor = (seq_t)2 * (1 + (seq_t)(3 * k));
        seq_t b_factor = (seq_t)(5 + 6 * k);

        num *= a_factor;
        den *= b_factor;

        /* reduce the fraction to keep numbers small */
        seq_t g = seq_gcd(num, den);
        num /= g;
        den /= g;
    }

    /* R(n) = (1/2)*(1 + P(n)) = (num + den) / (2*den) */
    seq_t sum = num + den;
    seq_t two_den = den * (seq_t)2;

    seq_t g2 = seq_gcd(sum, two_den);
    seq_t denom = two_den / g2;  /* the denominator of R(n) */

    return denom;
}
