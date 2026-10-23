/*
 * A035769  Coordination sequence for C_32 lattice.
 * https://oeis.org/A035769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^32
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 11, u128 n <= 52,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t two_n = 2 * n;
    idx_t max_i = (two_n < 32) ? two_n : 32;
    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t coeff1 = binomial(32, i);                /* C(32,i)   */
        idx_t  j     = two_n - i;                      /* exponent in (1-x)^(-32)  */
        seq_t coeff2 = binomial(31 + j, 31);           /* C(31+j,31) */
        result += coeff1 * coeff2;
    }
    return result;
}
