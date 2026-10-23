/*
 * A027765  a(n) = (n+1)*binomial(n+1,5).
 * https://oeis.org/A027765
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+1,5)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 90, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute a(n) = (n+1) * binomial(n+1, 5) */
    seq_t b = binomial(n + 1, 5);
    seq_t result = ((seq_t)(n + 1)) * b;
    return result;
}
