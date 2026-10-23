/*
 * A104673  a(n) = binomial(n+5, n)*binomial(n+10, n).
 * https://oeis.org/A104673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+5, n)*binomial(n+10, n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the two binomial coefficients and multiply them. */
    seq_t c1 = binomial(n + 5, n);
    seq_t c2 = binomial(n + 10, n);
    return c1 * c2;
}
