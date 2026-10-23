/*
 * A160039  Numerators of n!*(1 + 1/2 + 1/3 +...+ 1/(n+1)).
 * https://oeis.org/A160039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator of n! * (1 + 1/2 + 1/3 + … + 1/(n+1))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute factorial n! */
    seq_t fac = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fac *= (seq_t)i;
    }

    /* integer part sum_{k=1}^{n} fac / k  (all terms are integers) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += fac / (seq_t)k;
    }

    /* fraction part: fac / (n+1) */
    seq_t np1 = (seq_t)(n + 1);
    seq_t g = gcd(fac, np1);
    seq_t denom = np1 / g;   /* denominator after reducing the last fraction */
    seq_t f_red = fac / g;   /* numerator of the last fraction in reduced form */

    /* total numerator over the common denominator "denom" */
    seq_t numerator_val = sum * denom + f_red;

    /* reduce the final fraction */
    seq_t g2 = gcd(numerator_val, denom);
    return numerator_val / g2;
}
