/*
 * A093196  Number of 4 X 4 pandiagonal magic squares with line sum 2*n.
 * https://oeis.org/A093196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0}^{5} coeff_k * binomial(n+6-k, 6) where coeff =
 *            [1,21,90,90,21,1] (polynomial (1+x)(1+20x+70x²+20x³+x⁴)
 *            coefficients)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom6.h"

seq_t a(idx_t n)
{
    static const uint64_t coeff[6] = {1, 21, 90, 90, 21, 1};
    seq_t result = 0;
    for (int k = 0; k < 6; ++k) {
        idx_t N = n + 6 - k;    /* N = n+6-k */
        seq_t term = binom6(N);
        result += term * (seq_t)coeff[k];
    }
    return result;
}
