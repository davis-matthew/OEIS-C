/*
 * A027793  a(n) = 28*(n+1)*binomial(n+3,8)/3.
 * https://oeis.org/A027793
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,6) * binomial(n+3,3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The formula from OEIS can be rewritten as the product of two
       binomial coefficients:  C(n+1,6) * C(n+3,3).  The helper
       `binomial` already uses a multiplicative implementation that
       stays within range for the values that fit in seq_t. */
    return binomial(n + 1, 6) * binomial(n + 3, 3);
}
