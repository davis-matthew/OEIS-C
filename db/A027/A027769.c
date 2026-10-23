/*
 * A027769  a(n) = (n+1)*binomial(n+1, 9).
 * https://oeis.org/A027769
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+1, 9)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 35, u64 n <= 306,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t
a(idx_t n)
{
    /* For indices below the offset the value is undefined; return 0. */
    if (n < 8)
        return 0;

    /* Compute C(n+1, 9) using the provided utility routine. */
    seq_t c = binomial(n + 1, 9);

    /* Multiply by (n+1); cast to seq_t once to preserve type. */
    return (seq_t)(n + 1) * c;
}
