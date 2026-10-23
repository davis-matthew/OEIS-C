/*
 * A029253  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A029253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 4, 6, and 9
 * Range:     u8 n <= 86, u16 n <= 620, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
    /* For C++ we use the null pointer literal. */
    #define NULL_PTR nullptr
#else
    /* For C we use the standard NULL macro. */
    #define NULL_PTR 0
#endif

seq_t a(idx_t n) {
    static seq_t *dp = NULL_PTR;   /* cached DP array */
    static idx_t   max_n = 0;      /* highest n for which dp is valid */

    if (dp == NULL_PTR || n > max_n) {
        /* free previous memory */
#if defined(__cplusplus)
        if (dp) delete[] dp;
        dp = new seq_t[n + 1];
#else
        if (dp) free(dp);
        dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
#endif
        if (!dp) return (seq_t)0;   /* allocation failed */

        /* initialise all entries to zero */
        for (idx_t i = 0; i <= n; ++i) {
#if defined(__cplusplus)
            dp[i] = seq_t{};
#else
            dp[i] = 0;
#endif
        }
        dp[0] = 1;   /* one way to partition 0 */

        /* unbounded coin–change DP for parts 3,4,6,9 */
        static const idx_t parts[4] = {3, 4, 6, 9};
        for (int pi = 0; pi < 4; ++pi) {
            idx_t coin = parts[pi];
            for (idx_t i = coin; i <= n; ++i) {
                dp[i] += dp[i - coin];
            }
        }
        max_n = n;
    }

    return dp[n];
}
