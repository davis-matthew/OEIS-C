/*
 * A027774  a(n) = (n+1)*binomial(n+1,14).
 * https://oeis.org/A027774
 *
 * Offset:    13    (a(13) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * binomial(n+1, 14)
 * Range:     u8 n <= 14, u16 n <= 17, u32 n <= 28, u64 n <= 108,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute C(n+1, 14) */
    seq_t c = binomial(n + 1, 14);
    /* Multiply by (n+1) cast to seq_t */
    return c * ((seq_t)n + 1U);
}
