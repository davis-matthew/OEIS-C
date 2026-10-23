/*
 * A126922  Coordination sequence for 8-dimensional cyclotomic lattice
 *          Z[zeta_30].
 * https://oeis.org/A126922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..8} c[k] * binom(n+7-k, 7) with c = {1, 22, 208,
 *            874, 1480, 874, 208, 22, 1} derived from the generating function
 *            (x^8+22x^7+208x^6+874x^5+1480x^4+874x^3+208x^2+22x+1)/(1-x)^8
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 24, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static const unsigned short coeff[9] = {
        1, 22, 208, 874, 1480, 874, 208, 22, 1
    };
    seq_t result = 0;
    for (idx_t k = 0; k <= 8; ++k) {
        if (n < k) continue;
        idx_t m = n + 7 - k;               /* m >= 7 for binom(m,7) */
        seq_t b = binom(m, 7);             /* binom(m,7) */
        result += (seq_t)coeff[k] * b;     /* safe: coeff fits in 16 bits */
    }
    return result;
}
