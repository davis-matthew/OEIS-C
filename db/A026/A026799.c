/*
 * A026799  Number of partitions of n in which the least part is 6.
 * https://oeis.org/A026799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of partitions of n in which the least part is
 *            6. Computation: a(n) = number of partitions of (n-6) whose parts
 *            are all ≥6. The values are produced by dynamic programming on
 *            the cumulative sum (n-6). The table is grown lazily and cached
 *            for future calls
 * Range:     u8 n <= 49, u16 n <= 96, u32 n <= 226, u64 n <= 616,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* no partition with a minimum part of 6 for n<6 */
    if (n < 6)
        return (seq_t)0;

    /* the resulting number of partitions is stored in dp[sum] where
       sum = n-6.  dp[s] holds the count of partitions of s with all parts ≥6 */
    static idx_t max_sum = 0;     /* largest sum previously computed */
    static seq_t *dp    = NULL;   /* caching the DP array   */

    idx_t need = n - 6;           /* we need counts up to this sum   */

    /* Allocate (or reallocate) the DP array when we have a larger required sum.
       The first time this function is called with n≥6, dp will be NULL, so
       the condition catches that case too. */
    if (dp == NULL || need > max_sum) {
        size_t newsize = (size_t)(need + 1);            /* array length   */
        seq_t *newdp = NULL;
#if defined(__cplusplus)
        newdp = new seq_t[newsize]();   /* value‑initialised to 0 in C++ */
#else
        newdp = (seq_t*)calloc(newsize, sizeof(seq_t));
#endif
        if (!newdp)
            return (seq_t)0;            /* allocation failure – return 0 */

        /* one way to partition 0: the empty partition  */
        newdp[0] = (seq_t)1;

        /* compute partitions with parts ≥6 up to sum=need.
           We iterate over the allowed part sizes and perform the
           classic "unbounded knapsack" DP.  For each part we
           update all sums that can accommodate it. */
        for (idx_t part = 6; part <= need; ++part) {
            for (idx_t sum = part; sum <= need; ++sum) {
                /* Note: indices are small; conversion to size_t is safe. */
                newdp[(size_t)sum] += newdp[(size_t)(sum - part)];
            }
        }

        /* Replace the old table with the new one.  The old table is freed
           before updating the static pointer to avoid losing the pointer
           in case alloc/dealloc are non‑trivial in C++. */
#if defined(__cplusplus)
        delete[] dp;
#else
        free(dp);
#endif
        dp = newdp;
        max_sum = need;
    }

    /* A(n) is exactly the number of partitions of (n-6) with all parts
       ≥6, which is stored in dp[need]. */
    return dp[(size_t)need];
}
