/*
 * A173922  In the sequence of nonnegative integers substitute all n by
 *          2^floor(n/4) occurrences of (n mod 2).
 * https://oeis.org/A173922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the parity of the integer whose block contains n, where
 *            each integer k is replaced by 2^floor(k/4) copies of (k mod 2)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find q such that 4·2^q ≤ n+4 < 8·2^q.  This locates the group
       of four consecutive integers that contains n. */
    idx_t q = 0;
    idx_t limit = n + 4ULL;          /* n + 4 is safe even for n = 0 */
    while ((idx_t)4ULL << q <= limit) {
        q++;
    }
    q--;   /* last q satisfies the condition */

    /* Length of each block within this group: 2^q */
    idx_t block_len = (idx_t)1ULL << q;

    /* Starting index of this group: S(q) = 4·(2^q - 1) */
    idx_t group_start = 4ULL * (block_len - 1ULL);

    /* Position offset within the group */
    idx_t offset = n - group_start;          /* 0 … 4·block_len - 1 */

    /* Determine which of the four blocks holds the index n */
    idx_t block = offset / block_len;        /* 0,1,2,3 */

    /* Even block indices => parity 0; odd indices => parity 1 */
    return (seq_t)(block & 1U);
}
