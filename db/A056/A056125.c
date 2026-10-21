/*
 * A056125  a(n) = (5*n + 4)*binomial(n+7,7)/4.
 * https://oeis.org/A056125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*n + 4)*binomial(n+7,7)/4 Equivalent: a(n) =
 *            10*binomial(n+8,8) - 9*binomial(n+7,7)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 41, u64 n <= 719, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the provided binomial helper defined in the utils package. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the two needed binomial coefficients. */
    seq_t b1 = binomial(n + 7, 7);   /* C(n+7,7) */
    seq_t b2 = binomial(n + 8, 8);   /* C(n+8,8) */

    /* Return the value according to the equivalent formula. */
    return 10 * b2 - 9 * b1;
}
