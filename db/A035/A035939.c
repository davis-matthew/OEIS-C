/*
 * A035939  Number of partitions of n into parts not of the form 7k, 7k+3 or
 *          7k-3. Also number of partitions such that the differences between
 *          parts at distance 2 are greater than 1.
 * https://oeis.org/A035939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to 0,
 *            3, or 4 modulo 7 (i.e., parts of sizes 7k+1, 7k+2, 7k+5, or
 *            7k+6). Computed by a standard coin–change dynamic‑programming
 *            routine
 * Range:     u8 n <= 24, u16 n <= 66, u32 n <= 205, u64 n <= 687,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline int allowed_part(idx_t k) {
    idx_t r = k % 7;
    return (r != 0 && r != 3 && r != 4);
}

seq_t a(idx_t n) {
    /* Allocate a temporary array of size n+1.  The array holds values of type seq_t,
       which may be a primitive integer type or a big‑integer class.  The assignment
       dp[i] = 0; works for both by converting the literal 0 to seq_t. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* out of memory: return 0 */

    /* Initialise dp[0] = 1, all other entries = 0. */
    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Standard coin‑change DP: for each allowed part k, add its contributions. */
    for (idx_t k = 1; k <= n; ++k) {
        if (!allowed_part(k)) continue;
        for (idx_t i = k; i <= n; ++i) {
            dp[i] += dp[i - k];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
