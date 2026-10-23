/*
 * A161788  a(n) is the largest integer of the form 2^k - 1 that divides n.
 * https://oeis.org/A161788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest integer of the form 2^k - 1 that divides n
 * Range:     u8 n <= 509, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t ans = 1;                     /* at least 1 divides every n */

    for (idx_t k = 1; ; ++k) {
        seq_t m = pow2(k) - 1;         /* 2^k - 1 */
        if (m > nseq)                  /* further values exceed n */
            break;
        if (nseq % m == 0)             /* m divides n */
            ans = m;                   /* keep the largest such m */
    }
    return ans;
}
