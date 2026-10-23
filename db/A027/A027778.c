/*
 * A027778  a(n) = 5*(n+1)*binomial(n+2, 5)/2.
 * https://oeis.org/A027778
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,4) * binomial(n+2,2)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 76, u64 n <= 3098,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t bin4 = binomial(n + 1, 4);
    seq_t bin2 = binomial(n + 2, 2);
    return bin4 * bin2;
}
