/*
 * A007772  Values of Ehrhart polynomial of dilation by 2 of Relaxed Boolean
 *          Quadric Polytope of order 3.
 * https://oeis.org/A007772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 39*C(n,1) + 338*C(n,2) + 1140*C(n,3) + 1800*C(n,4) +
 *            1344*C(n,5) + 384*C(n,6)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   ehr_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ehr_binom.h"

seq_t a(idx_t n) {
    static const unsigned coef[7] = {1, 39, 338, 1140, 1800, 1344, 384};
    seq_t result = 0;
    idx_t k;
    for (k = 0; k <= 6; k++) {
        result += ehr_binom(n, k) * (seq_t)coef[k];
    }
    return result;
}
