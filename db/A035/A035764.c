/*
 * A035764  Coordination sequence for C_27 lattice.
 * https://oeis.org/A035764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^27. Computed as a(n) =
 *            Σ_{i=0}^{min(27,2n)} binom(27,i) * binom(26+2n-i, 26)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 13, u128 n <= 77,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t two_n = n + n;
    idx_t max_i = two_n < 27 ? two_n : 27;
    seq_t sum = 0;
    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t c1 = binom(27, i);
        idx_t j = two_n - i;
        seq_t c2 = binom(26 + j, 26);
        sum += c1 * c2;
    }
    return sum;
}
