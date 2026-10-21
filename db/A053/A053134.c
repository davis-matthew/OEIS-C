/*
 * A053134  Binomial coefficients C(2*n+4,4).
 * https://oeis.org/A053134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+4,4)
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 199, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(2*n+4, 4) using the supplied utility. */
    return binom(2*n + 4, 4);
}
