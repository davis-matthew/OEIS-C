/*
 * A133238  Dimensions of certain Lie algebra (see reference for precise
 *          definition).
 * https://oeis.org/A133238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{min(8,n)} c_k * binom(n+15−k,15) where c =
 *            {1,36,341,1208,1820,1208,341,36,1}
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 66, u128 n <= 1344,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"


static const int coeff[9] = {1, 36, 341, 1208, 1820, 1208, 341, 36, 1};

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= 8 && k <= n; ++k) {
        seq_t comb = binom(n + 15 - k, 15);
        sum += (seq_t)coeff[k] * comb;
    }
    return sum;
}
