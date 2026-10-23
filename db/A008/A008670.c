/*
 * A008670  Molien series for Weyl group F_4.
 * https://oeis.org/A008670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = number of partitions of n into parts {1,3,4,6}. (a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=2, a(4)=3, …)
 * Range:     u8 n <= 41, u16 n <= 297, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static buffer for dynamic programming */
    static seq_t *dp   = NULL;
    static idx_t  cap  = 0;   /* current capacity of dp */

    /* ensure the buffer is large enough for indices 0..n */
    if ((idx_t)(cap) <= n) {
        idx_t newcap = n + 1;
        seq_t *newdp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!newdp) return 0;            /* allocation fails → return 0 */
        if (dp) free(dp);
        dp   = newdp;
        cap  = newcap;
    }

    /* initialize dp[0..n] to 0, then set dp[0] = 1 */
    memset(dp, 0, (size_t)(n + 1) * sizeof(seq_t));
    dp[0] = 1;

    /* the coin types (part sizes) */
    static const idx_t parts[] = {1, 3, 4, 6};
    static const size_t parts_cnt = sizeof(parts) / sizeof(parts[0]);

    /* for each part, add its contributions to all achievable sums */
    for (size_t k = 0; k < parts_cnt; ++k) {
        idx_t p = parts[k];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    return dp[n];
}
