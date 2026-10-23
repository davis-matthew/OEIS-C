/*
 * A017781  Binomial coefficients C(65,n).
 * https://oeis.org/A017781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(65,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Binomial coefficient C(65,n) is zero for n > 65 */
    if (n > 65) return (seq_t)0;

    /* Use symmetry C(65,n) = C(65,65-n) to minimize steps */
    idx_t k = n;
    if (k > 65 - k) k = 65 - k;

    seq_t res = (seq_t)1;

    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        seq_t num = (seq_t)(65 - k + i);
        seq_t den = (seq_t)i;

        /* Cancel common factors between current result and denominator */
        seq_t g1 = gcd((seq_t)res, den);
        res /= g1;
        den /= g1;

        /* Cancel remaining common factors between numerator and denominator */
        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;   /* should become 1 after this step */

        /* Multiply the remaining numerator factor */
        res *= num;

        /* If any denominator remains (should not happen), divide it out */
        if (den != (seq_t)1)
            res /= den;
    }

    return res;
}
