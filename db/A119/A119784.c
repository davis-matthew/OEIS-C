/*
 * A119784  Numerator of Sum[Sum[(-1)^(i+1)*1/(i*j)^2, {i, 1, n}], {j, 1, n}].
 * https://oeis.org/A119784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of ( Σ_{i=1..n} (-1)^{i+1}/i^2 ) · ( Σ_{j=1..n}
 *            1/j^2 )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute the alternating zeta(2) sum S = Σ (-1)^{i+1}/i^2  */
    seq_t numS = 0;
    seq_t denS = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sign = (i & 1) ? 1 : -1;          /* (-1)^{i+1} */
        seq_t termDen = (seq_t)i * (seq_t)i;    /* i^2 */
        /* add sign/termDen to numS/denS */
        seq_t newNum = numS * termDen + denS * sign;
        seq_t newDen = denS * termDen;
        seq_t g = gcd(newNum < 0 ? -newNum : newNum, newDen);
        if (g > 1) {
            newNum /= g;
            newDen /= g;
        }
        numS = newNum;
        denS = newDen;
    }

    /* Compute the harmonic zeta(2) sum H = Σ 1/j^2  */
    seq_t numH = 0;
    seq_t denH = 1;

    for (idx_t j = 1; j <= n; ++j) {
        seq_t termDen = (seq_t)j * (seq_t)j;    /* j^2 */
        seq_t newNum = numH * termDen + denH * 1;
        seq_t newDen = denH * termDen;
        seq_t g = gcd(newNum, newDen);
        if (g > 1) {
            newNum /= g;
            newDen /= g;
        }
        numH = newNum;
        denH = newDen;
    }

    /* Multiply fractions S * H and cancel common factor */
    seq_t prodNum = numS * numH;
    seq_t prodDen = denS * denH;
    seq_t g = gcd(prodNum < 0 ? -prodNum : prodNum, prodDen);
    if (g > 1)
        prodNum /= g;

    /* The problem asks for the numerator; for n ≥ 1 it is always positive */
    return prodNum < 0 ? -prodNum : prodNum;
}
