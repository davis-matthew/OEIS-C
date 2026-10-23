/*
 * A017739  Binomial coefficients C(n,75).
 * https://oeis.org/A017739
 *
 * Offset:    75    (a(75) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,75) for n ≥ 75
 * Range:     u8 n <= 76, u16 n <= 77, u32 n <= 82, u64 n <= 93, u128 n <= 133,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 75; for n < 75 we return 0
       (the tests never query these values). */
    if (n < 75)
        return 0;

    const idx_t k = 75;
    seq_t res = 1;

    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);   /* numerator term */
        seq_t den = (seq_t)i;             /* denominator term */

        /* Reduce gcd between current result and denominator to prevent overflow. */
        seq_t g1 = gcd(res, den);
        res = res / g1;
        den = den / g1;

        /* Further reduce between numerator and remaining denominator. */
        seq_t g2 = gcd(num, den);
        num = num / g2;
        den = den / g2;

        res = res * num;
        res = res / den;   /* exact division */
    }

    return res;
}
