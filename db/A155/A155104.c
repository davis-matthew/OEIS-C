/*
 * A155104  Numbers appearing in the fourth column of A155103.
 * https://oeis.org/A155104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prod(k=2..n) (1+2^k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* not used; negligeable safety */

    seq_t result = (seq_t)1;
    seq_t pow2   = (seq_t)2;   /* 2^1 */

    for (idx_t k = 2; k <= n; ++k)
    {
        /* compute 2^k by doubling */
        pow2 = pow2 + pow2;          /* pow2 *= 2 */
        seq_t term = pow2 + (seq_t)1; /* (1 + 2^k) */
        result = result * term;      /* accumulate product */
    }

    return result;
}
