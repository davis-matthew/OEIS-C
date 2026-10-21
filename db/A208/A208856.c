/*
 * A208856  Partitions of n into parts not congruent to 0, +-4, +-6, +-10, 16
 *          (mod 32).
 * https://oeis.org/A208856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts not congruent to *
 *            0, ±4, ±6, ±10, 16 (mod 32)
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 160, u64 n <= 532,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const unsigned char allowed[32] = {
    0, 1, 1, 1, 0, 1, 0, 1,
    1, 1, 0, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 0, 1,
    1, 1, 0, 1, 0, 1, 1, 1
};

seq_t a(idx_t n)
{
    /* dp array holds number of partitions for each sum up to n */
    size_t len = (size_t)n + 1;
    seq_t *dp;
#if defined(__cplusplus)
    dp = new seq_t[len];
#else
    dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) exit(EXIT_FAILURE);
#endif

    /* initialize */
    for (size_t i = 0; i < len; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* unbounded knapsack over allowed part sizes */
    for (idx_t p = 1; p <= n; ++p) {
        if (!allowed[p & 31]) continue;  /* forbidden residue */
        for (idx_t j = p; j <= n; ++j) {
            dp[j] = dp[j] + dp[j - p];
        }
    }

    seq_t result = dp[n];

#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
