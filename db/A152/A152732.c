/*
 * A152732  a(n) + a(n+1) + a(n+2) = 2^n.
 * https://oeis.org/A152732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) + a(n+1) + a(n+2) = 2^n (OEIS A152732, offset 1)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 2)               /* a(1) = a(2) = 0 */
        return 0;

    /* Sliding window recurrence:
       a(k+2) = 2^k - a(k) - a(k+1) */
    seq_t prev2 = 0; /* a(1) */
    seq_t prev1 = 0; /* a(2) */
    seq_t cur   = 0; /* will become a(k+2) */

    for (idx_t k = 1; k <= n - 2; ++k)
    {
        cur = pow2(k) - prev2 - prev1;  /* a(k+2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur; /* a(n) */
}
