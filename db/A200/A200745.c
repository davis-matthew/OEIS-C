/*
 * A200745  Number of partitions of n into distinct non-divisors of n.
 * https://oeis.org/A200745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into distinct parts that are not
 *            divisors of n
 * Range:     u8 n <= 36, u16 n <= 88, u32 n <= 250, u64 n <= 820,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Base case: empty partition counts as one way for n = 0 */
    if (n == 0)
        return 1;

    /* The DP array holds the number of ways to reach each sum
     * using already considered parts.  DP[0] starts at 1 (empty sum). */
#ifdef __cplusplus
    /* Use std::vector for C++ builds (supports arbitrary‑precision types). */
    std::vector<seq_t> dp(static_cast<size_t>(n) + 1);
    dp[0] = 1;
    for (idx_t k = 1; k < n; ++k) {
        /* Skip parts that divide n. */
        if (n % k == 0)
            continue;
        /* 0/1 knapsack update: go backwards to enforce distinctness. */
        for (idx_t sum = n; sum >= k; --sum) {
            dp[sum] += dp[sum - k];
        }
    }
    return dp[n];
#else
    /* Use malloc/calloc for pure C builds (built‑in integer seq_t). */
    seq_t *dp = calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp)
        return 0;           /* allocation failure – return 0 as a safe default */
    dp[0] = 1;
    for (idx_t k = 1; k < n; ++k) {
        if (n % k == 0)
            continue;
        for (idx_t sum = n; sum >= k; --sum) {
            dp[sum] += dp[sum - k];
        }
    }
    seq_t result = dp[n];
    free(dp);
    return result;
#endif
}
