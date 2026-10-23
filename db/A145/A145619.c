/*
 * A145619  Numerator of the polynomial A_l(x) = sum_{d=1..l-1} x^(l-d)/d for
 *          index l=2n+1 evaluated at x=6.
 * https://oeis.org/A145619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of Σ_{d=1}^{2n} 6^(2n+1‑d)/d
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* l = 2*n + 1  (fits into idx_t for the intended range) */
    idx_t l = n * 2 + 1;

    /* pow_high = 6^(l-1) */
    seq_t pow_high = (seq_t)1;
    const seq_t six = (seq_t)6;
    for (idx_t i = 0; i < l - 1; ++i) {
        pow_high *= six;
    }

    seq_t num = (seq_t)0;   /* numerator of the running sum */
    seq_t den = (seq_t)1;   /* denominator of the running sum */

    for (idx_t d = 1; d < l; ++d) {
        seq_t term_num = pow_high;      /* 6^(l-d)   */
        pow_high /= six;                /* next power */

        /* New fraction: (num/den) + (term_num/d) */
        seq_t new_num = num * (seq_t)d + term_num * den;
        seq_t new_den = den * (seq_t)d;

        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;    /* Return the numerator in lowest terms */
}
