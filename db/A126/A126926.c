/*
 * A126926  Coordination sequence for 12-dimensional cyclotomic lattice
 *          Z[zeta_36].
 * https://oeis.org/A126926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{12} coeffs[i] · C(11 + n − i, 11)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where coeffs are the coefficients of (1 + 4x + x²)⁶           */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* coefficients of (1 + 4x + x²)⁶, degree 12 */
    static const unsigned long long coeffs[13] = {
        1ULL, 24ULL, 246ULL, 1400ULL, 4815ULL, 10224ULL,
        13236ULL, 10224ULL, 4815ULL, 1400ULL, 246ULL, 24ULL, 1ULL
    };

    seq_t result = 0;
    idx_t i;

    for (i = 0; i < 13; ++i) {
        if (n < i) continue;                     /* skip terms beyond n   */
        seq_t bin   = binom(11 + n - i, 11);     /* C(11 + n − i, 11)    */
        result += (seq_t)coeffs[i] * bin;         /* accumulate contribution */
    }

    return result;
}
