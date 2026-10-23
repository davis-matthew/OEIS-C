/*
 * A107422  a(n) = binomial(n+7,7) * binomial(n+10,10).
 * https://oeis.org/A107422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+7,7) * binomial(n+10,10)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 49, u128 n <= 736,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the two binomial coefficients and multiply them.
       The helper function binomial(idx_t N, idx_t K) returns a
       seq_t and performs the computation in a safe, multiplicative
       manner, avoiding overflow of intermediate factorials. */
    seq_t c1 = binomial(n + 7, 7);
    seq_t c2 = binomial(n + 10, 10);
    return c1 * c2;
}
