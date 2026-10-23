/*
 * A027800  a(n) = (n+1)*binomial(n+4, 4).
 * https://oeis.org/A027800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+4, 4)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 157, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute C(n+4, 4) first */
    seq_t binom = binomial(n + 4, 4);

    /* Multiply by (n+1), converting to seq_t for exactness */
    return binom * ((seq_t)(n + 1));
}
