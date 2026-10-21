/*
 * A060061  Fourth column of triangle A060058.
 * https://oeis.org/A060061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6,6)*(280*n^3+2436*n^2+5906*n+3843)/(7*9)
 * Range:     u8 none, u16 n <= 2, u32 n <= 17, u64 n <= 240, u128 n <= 5000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    /* compute binomial(n+6,6) multiplicatively */
    seq_t binom = 1;
    for (idx_t k = 1; k <= 6; ++k) {
        binom = binom * (sn + (seq_t)(7 - k)) / (seq_t)k;
    }

    /* polynomial term */
    seq_t poly = 280 * sn * sn * sn
               + 2436 * sn * sn
               + 5906 * sn
               + 3843;

    /* divide by 63 using gcd reductions to avoid overflow */
    seq_t g1 = gcd(binom, (seq_t)63);
    binom /= g1;
    seq_t den1 = (seq_t)63 / g1;

    seq_t g2 = gcd(poly, den1);
    poly /= g2;
    /* den1 / g2 should now be 1 */

    return binom * poly;
}
