/*
 * A017735  Binomial coefficients C(n,71).
 * https://oeis.org/A017735
 *
 * Offset:    71    (a(71) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,71)
 * Range:     u8 n <= 72, u16 n <= 74, u32 n <= 78, u64 n <= 90, u128 n <= 132,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* undefined for indices below the offset – return 0 */
    if (n < 71) return 0;

    const idx_t k = 71;          /* fixed k = 71 */
    seq_t res = 1;                /* running product */

    for (idx_t i = 1; i <= k; ++i)
    {
        /* numerator and denominator for the current factor */
        seq_t num = (seq_t)(n - k + i);   /* n - 71 + i */
        seq_t den = (seq_t)i;

        /* cancel common factors between numerator and denominator */
        seq_t g1 = gcd(num, den);
        if (g1 != 1) { num /= g1; den /= g1; }

        /* cancel remaining denominator factors with the product so far */
        seq_t g2 = gcd(res, den);
        if (g2 != 1) { res /= g2; den /= g2; }

        /* at this point den should be 1; multiply the reduced numerator */
        res = res * num;
    }

    return res;
}
