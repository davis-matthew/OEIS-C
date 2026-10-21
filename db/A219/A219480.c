/*
 * A219480  Number of representations of n as a sum of distinct elements of
 *          the generalized Fibonacci sequence beginning 1, 7, 8, 15, 23, 38,
 *          ....
 * https://oeis.org/A219480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9735, u16 n <= 9735, u32 n <= 9735, u64 n <= 9735,
 *            u128 n <= 9735, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of representations of n as a sum of distinct
 * elements of the generalized Fibonacci sequence 1,7,8,15,23,38,...
 * Computed by dynamic programming: classic subset‑sum count.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Gather all sequence elements <= n.  The sequence follows
     * f(0)=1, f(1)=7, f(k)=f(k-1)+f(k-2).  Its size is tiny
     * (≈ log_φ n) so a small fixed array is sufficient. */
    idx_t fib[64];
    int cnt = 0;

    if (n >= 1) fib[cnt++] = 1;
    if (n >= 7) fib[cnt++] = 7;
    while (cnt >= 2) {
        idx_t next = fib[cnt - 2] + fib[cnt - 1];
        if (next > n) break;
        fib[cnt++] = next;
    }

    /* Allocate DP table: dp[s] = number of subsets summing to s. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) return 0;           /* fallback in the unlikely event of OOM */

    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                   /* empty subset sums to 0 */

    /* Process each term exactly once (distinctness) using a reverse
     * loop to avoid reusing a term. */
    for (int k = 0; k < cnt; ++k) {
        idx_t v = fib[k];
        for (idx_t s = n; s >= v; --s) {
            dp[s] += dp[s - v];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
