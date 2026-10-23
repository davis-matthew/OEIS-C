/*
 * A126903  Coordination sequence for 10-dimensional cyclotomic lattice
 *          Z[zeta_22].
 * https://oeis.org/A126903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{10} c_i · binom(n - i + 9, 9) , where c =
 *            {1,12,67,232,562,1024,562,232,67,12,1}
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static const idx_t coeff[11] = {1,12,67,232,562,1024,562,232,67,12,1};
    seq_t result = 0;

    for (idx_t i = 0; i < 11; ++i) {
        if (n >= i) {
            seq_t term = binom(n - i + 9, 9);
            result += (seq_t)coeff[i] * term;
        }
    }

    return result;
}
