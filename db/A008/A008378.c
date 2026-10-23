/*
 * A008378  Coordination sequence for D_10 lattice.
 * https://oeis.org/A008378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0}^{10} c_j * binomial(n - j + 9, 9) where c =
 *            [1,170,4685,38200,124850,183356,124850,38200,4685,170,1]
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 11, u64 n <= 132, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static const unsigned int coeff[11] = {
        1U,      170U,     4685U,    38200U,   124850U,
        183356U, 124850U,  38200U,   4685U,    170U,
        1U
    };

    seq_t result = 0;
    for (int j = 0; j <= 10; ++j) {
        if (n < (idx_t)j) continue;
        idx_t m = n - (idx_t)j;
        seq_t comb = binomial(m + 9, 9);
        result += (seq_t)coeff[j] * comb;
    }
    return result;
}
