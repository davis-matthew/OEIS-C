/*
 * A067595  Number of partitions of n into distinct Lucas parts (A000032).
 * https://oeis.org/A067595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = number of subsets of distinct Lucas numbers that sum to n
 *            (A067595)
 * Range:     u8 n <= 9761, u16 n <= 9761, u32 n <= 9761, u64 n <= 9761,
 *            u128 n <= 9761, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;   /* dp[s] = number of subsets of Lucas numbers <= current limit that sum to s */
    static idx_t dp_limit = 0; /* largest n for which dp has been computed */

    /* Recompute dp only when we need a larger n. */
    if (dp == NULL || n > dp_limit) {
        /* Allocate array for sums 0..n. */
        size_t size = (size_t)(n + 1);
        seq_t *newdp = (seq_t *)calloc(size, sizeof(seq_t));
        if (!newdp) return 0;           /* allocation failure – should not happen */
        newdp[0] = (seq_t)1;            /* empty subset sums to 0 */

        /* Generate Lucas numbers and update DP. */
        idx_t l0 = (idx_t)2;            /* L0 */
        idx_t l1 = (idx_t)1;            /* L1 */

        if (l0 <= n) {
            for (idx_t s = n; s >= l0; --s)
                newdp[s] += newdp[s - l0];
        }
        if (l1 <= n && l1 != l0) {
            for (idx_t s = n; s >= l1; --s)
                newdp[s] += newdp[s - l1];
        }

        idx_t prev = l0;
        idx_t curr = l1;
        while (1) {
            idx_t next = prev + curr;   /* next Lucas number */
            if (next > n) break;
            for (idx_t s = n; s >= next; --s)
                newdp[s] += newdp[s - next];
            prev = curr;
            curr = next;
        }

        free(dp);
        dp = newdp;
        dp_limit = n;
    }

    return dp[n];
}
