/*
 * A185227  Number of disconnected 2-regular simple graphs on n vertices with
 *          girth at least 7.
 * https://oeis.org/A185227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n with all parts ≥7 and with at
 *            least two parts (i.e., A185327(n) – A185117(n))
 * Range:     u8 n <= 47, u16 n <= 97, u32 n <= 234, u64 n <= 641,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Forward declaration of the external function. */
seq_t a(idx_t n);

/* Static table for the partition counts with parts ≥7.  */
static seq_t *dp = NULL;
static idx_t dp_max = 0;

/* Compute dp[0..n], where dp[i] counts partitions of i using parts ≥7. */
static void
ensure_dp(idx_t n)
{
    if (dp_max >= n)
        return;                           /* already computed */

#ifdef __cplusplus
    /* Destroy old objects if any. */
    for (idx_t i = 0; i < dp_max; ++i)
        dp[i].~seq_t();

    free(dp);
    dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        exit(1);

    /* Construct seq_t objects. */
    for (idx_t i = 0; i <= n; ++i)
        new(&dp[i]) seq_t();
#else
    free(dp);
    dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        exit(1);
#endif

    /* Base cases. */
    dp[0] = 1;
    if (n >= 1)
        for (idx_t i = 1; i <= n; ++i)
            dp[i] = 0;

    /* Unbounded knapsack DP for parts ≥7. */
    for (idx_t part = 7; part <= n; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            dp[sum] = dp[sum] + dp[sum - part];
        }
    }

    dp_max = n;
}

/* External linkage function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 7)       /* no partitions with at least two parts */
        return 0;

    ensure_dp(n);

    /* dp[n] includes the single-part partition [n], subtract it. */
    seq_t res = dp[n] - 1;
    return res;
}
