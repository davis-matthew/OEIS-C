/*
 * A107509  Coordination sequence for D_14 lattice.
 * https://oeis.org/A107509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0}^{14} c[j] * binom(n - j + 13, 13) where * c = [1,
 *            350, 20139, 374892, 3101945, 13109250, 30399579, 40090728, *
 *            30399579, 13109250, 3101945, 374892, 20139, 350, 1]. * This
 *            implements the rational generating function * (x^14 + 350x^13 +
 *            … + 350x + 1) / (1 - x)^14
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static const uint64_t coeff[15] = {
        1ULL, 350ULL, 20139ULL, 374892ULL, 3101945ULL,
        13109250ULL, 30399579ULL, 40090728ULL, 30399579ULL,
        13109250ULL, 3101945ULL, 374892ULL, 20139ULL,
        350ULL, 1ULL
    };

    seq_t result = 0;
    for (idx_t j = 0; j < 15; ++j) {
        if (n < j) continue;               /* term vanishes if n < j */
        idx_t m = n - j + 13;               /* binomial argument */
        seq_t term = binom(m, 13);
        term *= (seq_t)coeff[j];
        result += term;
    }
    return result;
}
