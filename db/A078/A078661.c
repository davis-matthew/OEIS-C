/*
 * A078661  a(0) = 1; for n>0, a(n) = 1 + coefficient of x^n in expansion of
 *          Product_{ n >= 2, n not of the form 2^k-1 } (1+x^n).
 * https://oeis.org/A078661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + number of subsets of distinct integers m ≥ 2, not of
 *            the form 2^k-1, whose sum equals n. The subset-sum generating
 *            function is Product_{m≥2, m≠2^k-1}(1+x^m). This implementation
 *            uses a classic DP that counts all such subsets
 * Range:     u8 n <= 41, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Quick answer for the first term */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate DP array of size n+1 */
    const size_t N = (size_t)n;
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) {
        return (seq_t)0;  /* allocation failure – unlikely for expected inputs */
    }

    /* Initialize DP */
    for (size_t i = 0; i <= N; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;  /* empty subset */

    /* Iterate over all allowed integers m */
    for (idx_t m = 2; m <= n; ++m) {
        /* Skip m if m+1 is a power of two: that means m = 2^k-1 */
        if (((m + 1) & m) == 0) {
            continue;
        }

        /* Update DP from high to low to maintain distinctness */
        for (idx_t j = n; j >= m; --j) {
            dp[(size_t)j] += dp[(size_t)(j - m)];
        }
    }

    /* The desired value is 1 + number of subsets summing to n */
    seq_t result = dp[N] + 1;

    /* Clean up */
    free(dp);
    return result;
}
