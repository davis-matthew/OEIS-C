/*
 * A035759  Coordination sequence for C_22 lattice.
 * https://oeis.org/A035759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^22
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 16, u128 n <= 143,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides static seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t two_n = n * 2;                      /* safe for all n used */
    idx_t max_i = two_n < 22 ? two_n : (idx_t)22;
    for (idx_t i = 0; i <= max_i; ++i)
    {
        seq_t c1 = binomial(22, i);          /* (1+x)^22 coefficient */
        idx_t j  = two_n - i;                /* exponent from (1-x)^(-22) */
        idx_t m  = 21 + j;                   /* m = 21 + j */
        seq_t c2 = binomial(m, 21);          /* (1-x)^(-22) coefficient */
        total += c1 * c2;
    }
    return total;
}
