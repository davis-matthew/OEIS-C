/*
 * A035771  Coordination sequence for C_34 lattice.
 * https://oeis.org/A035771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^34
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t two_n = 2 * n;
    seq_t sum = 0;
    idx_t max_i = two_n < 34 ? two_n : 34;
    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t k = two_n - i;
        seq_t term = binomial(34, i) * binomial(33 + k, k);
        sum += term;
    }
    return sum;
}
