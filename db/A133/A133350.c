/*
 * A133350  Dimensions of certain Lie algebra (see reference for precise
 *          definition).
 * https://oeis.org/A133350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^{5} c[i] · binomial(n+7-i, 7) with coefficients c
 *            = [1, 82, 582, 840, 273, 14]
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 26, u64 n <= 654,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static const unsigned int coeff[6] = {1u, 82u, 582u, 840u, 273u, 14u};
    seq_t sum = 0;
    for (idx_t i = 0; i < 6; ++i)
    {
        sum += (seq_t)coeff[i] * binom(n + 7 - i, 7);
    }
    return sum;
}
