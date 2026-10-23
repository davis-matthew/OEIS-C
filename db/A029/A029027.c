/*
 * A029027  Expansion of 1/((1-x)*(1-x^2)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   partitions of n into parts 1, 2, 9 and 10
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of ways to write n as a sum of 1,2,9,10 (order does not matter) */

#include <stdlib.h>

/* Index type from build (uint64_t) and value type (seq_t). */
seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* Allocate dynamic array for DP, type depends on build language. */
#ifdef __cplusplus
    seq_t *dp = new seq_t[(size_t)n + 1];
#else
    seq_t *dp = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
#endif
    if (!dp)
        return (seq_t)0;           /* allocation failure */

    dp[0] = (seq_t)1;               /* one way to partition 0 */

    /* The allowed part sizes. */
    const idx_t parts[4] = {1, 2, 9, 10};

    /* DP: for each part size add contributions from smaller sums. */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t part = parts[pi];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];

    /* Free the allocated memory. */
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
