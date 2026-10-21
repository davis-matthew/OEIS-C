/*
 * A085374  a(n) = binomial(2n+1, n+1)*binomial(n+3, 3).
 * https://oeis.org/A085374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2*n+1, n+1)*binomial(n+3, 3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the two binomial coefficients using the library helper */
    seq_t b1 = binomial((idx_t)(2 * n + 1), (idx_t)(n + 1)); /* C(2n+1, n+1) */
    seq_t b2 = binomial((idx_t)(n + 3), (idx_t)3);            /* C(n+3, 3) */

    /* Multiply them.  The library handles overflow transparently:
       - for the arbitrary‑precision build it uses big integer multiplication
       - for the 64‑bit build, if the result fits in 64 bits it will not overflow. */
    return b1 * b2;
}
