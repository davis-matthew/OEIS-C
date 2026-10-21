/*
 * A085373  a(n) = binomial(2n+1, n+1)*binomial(n+2, 2).
 * https://oeis.org/A085373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2*n+1, n+1) * binomial(n+2, 2)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute C(2n+1, n+1) */
    seq_t c1 = binomial(2*n + 1, n + 1);

    /* Compute C(n+2, 2) – equals (n+2)(n+1)/2 */
    seq_t c2 = binomial(n + 2, 2);

    return c1 * c2;
}
