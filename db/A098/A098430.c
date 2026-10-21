/*
 * A098430  a(n) = 4^n*(2*n)!/(n!)^2.
 * https://oeis.org/A098430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n*(2*n)!/(n!)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Base case n = 0 */
    if (n == 0)
        return 1;

    /* Compute C(2*n, n) */
    seq_t comb = binomial(2 * n, n);

    /* Compute 4^n = 2^(2*n) */
    seq_t p = pow2(2 * n);

    /* Return product */
    return p * comb;
}
