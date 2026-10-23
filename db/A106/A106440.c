/*
 * A106440  a(n) = binomial(2n+4,n)*binomial(n+4,4).
 * https://oeis.org/A106440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2n+4,n) * binomial(n+4,4)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute the two binomial coefficients */
    idx_t N1 = 2 * n + 4;          /* 2n + 4 */
    seq_t c1 = binom(N1, n);      /* binomial(2n+4, n) */

    idx_t N2 = n + 4;              /* n + 4 */
    seq_t c2 = binom(N2, 4);       /* binomial(n+4, 4) */

    return c1 * c2;                /* product */
}
