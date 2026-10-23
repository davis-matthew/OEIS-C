/*
 * A144485  a(n) = (3n + 2)*binomial(3n + 1,n).
 * https://oeis.org/A144485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3n+2)*binomial(3n+1,n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t factor = (seq_t)(3ULL * n + 2);
    seq_t bin    = binomial(3ULL * n + 1, n);
    return factor * bin;
}
