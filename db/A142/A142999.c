/*
 * A142999  a(0) = 0, a(1) = 1; for n > 1, a(n+1) = (2*n + 1)*a(n) +
 *          n^4*a(n-1).
 * https://oeis.org/A142999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(0) = 0, a(1) = 1, a(n+1) = (2*n + 1)*a(n) + n^4*a(n-1)
 * (OEIS A142999)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* keep only the last two values */
    seq_t prev2 = 0;   /* a(n-1) */
    seq_t prev1 = 1;   /* a(n)   */

    for (idx_t k = 1; k < n; ++k)
    {
        seq_t kseq = (seq_t)k;
        /* k^4 */
        seq_t k2   = kseq * kseq;
        seq_t k4   = k2 * k2;

        seq_t cur = (2 * kseq + 1) * prev1 + k4 * prev2;

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;   /* a(n) */
}
