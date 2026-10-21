/*
 * A092321  Sum of largest parts (counted with multiplicity) of all partitions
 *          of n.
 * https://oeis.org/A092321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=1}^{n} k * sum_{m=1}^{⌊n/k⌋} m * P_{k-1}(n - m*k),
 *            where P_{j}(s) is the number of partitions of s with parts ≤ j
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 99, u64 n <= 353,
 *            u128 n <= 1317, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A092321: Sum of largest parts (counted with multiplicity)
   of all partitions of n. */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;

    /* Allocate array holding partition counts for parts ≤ current size. */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;           /* allocation failed */

    dp[0] = (seq_t)1;                   /* only one way to partition zero */

    seq_t ans = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        /* Contribution of partitions whose largest part is k.
           Currently dp[s] = P_{k-1}(s). */
        for (idx_t m = 1; ; ++m) {
            idx_t prod = m * k;
            if (prod > n) break;        /* m*k > n => no more terms */
            idx_t s = n - prod;          /* remaining sum */
            seq_t cnt = dp[s];           /* P_{k-1}(s) */
            if (cnt != (seq_t)0) {
                seq_t contrib = cnt * (seq_t)k;
                contrib *= (seq_t)m;
                ans += contrib;
            }
        }

        /* Update dp to count partitions with parts ≤ k. */
        for (idx_t s = k; s <= n; ++s) {
            dp[s] += dp[s - k];
        }
    }

    free(dp);
    return ans;
}
