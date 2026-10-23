/*
 * A027767  a(n) = (n+1)*binomial(n+1,7).
 * https://oeis.org/A027767
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * binomial(n+1, 7)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(n+1, 7) */
    seq_t c = binomial((idx_t)(n + 1), (idx_t)7);

    /* Multiply by (n+1) as seq_t */
    return ((seq_t)(n + 1)) * c;
}
