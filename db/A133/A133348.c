/*
 * A133348  Dimensions of certain Lie algebra (see reference for precise
 *          definition).
 * https://oeis.org/A133348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0..6} c_i * binomial(n + 15 - i, 15) , where c = {1,
 *            16, 70, 112, 70, 16, 1}. This follows from the generating
 *            function (x^6+16x^5+70x^4+112x^3+70x^2+16x+1)/(1-x)^16
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 14, u64 n <= 79, u128 n <= 1627,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static const int coeff[7] = {1, 16, 70, 112, 70, 16, 1};
    seq_t result = 0;
    idx_t i;
    for (i = 0; i < 7u; ++i) {
        if (n >= i) {
            seq_t term = binomial(n + 15 - i, 15);
            result += term * (seq_t)coeff[i];
        }
    }
    return result;
}
