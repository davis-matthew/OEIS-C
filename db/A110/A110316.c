/*
 * A110316  a(n) is the number of different shapes of balanced binary trees
 *          with n nodes. The tree is balanced if the total number of nodes in
 *          the left and right branch of every node differ by at most one.
 * https://oeis.org/A110316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(0) = a(1) = 1; a(2*n) = 2 * a(n) * a(n-1); a(2*n+1) = a(n) *
 *            a(n);
 * Range:     u8 n <= 17, u16 n <= 36, u32 n <= 73, u64 n <= 148,
 *            u128 n <= 297, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;
    static idx_t max_n   = 0;

    /* Ensure array is at least size n+1, but not less than 2 */
    if (dp == NULL) {
        dp_size = (n + 1 > 2) ? (idx_t)(n + 1) : 2;
        dp = (seq_t*)malloc((size_t)dp_size * sizeof(seq_t));
        for (idx_t i = 0; i < dp_size; ++i) dp[i] = (seq_t)0;
        dp[0] = dp[1] = (seq_t)1;      /* initial values */
        max_n = 1;
    }

    if (n > max_n) {
        /* Extend the array if necessary */
        if (dp_size <= n) {
            size_t new_size = (size_t)(n + 1);
            seq_t *tmp = (seq_t*)realloc(dp, new_size * sizeof(seq_t));
            if (tmp == NULL) abort();     /* allocation failure */
            dp = tmp;
            for (idx_t i = dp_size; i < new_size; ++i) dp[i] = (seq_t)0;
            dp_size = new_size;
        }
        /* Compute missing entries */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i < 2) continue;          /* safety; should not happen */
            if ((i & 1) == 0) {
                idx_t k = i >> 1;          /* i == 2*k */
                seq_t left  = dp[k];
                seq_t right = (k >= 1) ? dp[k - 1] : (seq_t)1;
                dp[i] = (seq_t)2 * left * right;
            } else {
                idx_t k = i >> 1;          /* i == 2*k + 1 */
                seq_t v = dp[k];
                dp[i] = v * v;
            }
        }
        max_n = n;
    }

    return dp[n];
}
