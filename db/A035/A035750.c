/*
 * A035750  Coordination sequence for C_13 lattice.
 * https://oeis.org/A035750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=0}^{min(13,2n)} binom(13,i)*binom(12+2n-i, 2n-i)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 50, u128 n <= 2028,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t m = n << 1;                 /* m = 2*n */
    idx_t limit = 13;
    if (m < limit) limit = m;         /* iterate i = 0 .. limit */
    seq_t sum = 0;
    for (idx_t i = 0; i <= limit; ++i)
    {
        seq_t term1 = binom(13, i);
        seq_t term2 = binom(12 + m - i, m - i);
        sum += term1 * term2;
    }
    return sum;
}
