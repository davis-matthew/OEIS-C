/*
 * A174002  a(n) = n*binomial(n+4, 4).
 * https://oeis.org/A174002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * binomial(n + 4, 4)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n = 0 the result is 0, since binomial(4,4) = 1 */
    return (seq_t)n * binomial(n + 4, 4);
}
