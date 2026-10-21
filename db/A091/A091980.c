/*
 * A091980  Recursive sequence; one more than maximum of products of pairs of
 *          previous terms with indices summing to current index.
 * https://oeis.org/A091980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + max_{i=1..n-1} a(i)*a(n-i) To compute efficiently we
 *            keep a table of all previously computed terms and use the
 *            observation that the maximum product is always achieved for i
 *            equal to the power of two that is closest to n/2. If two such
 *            powers exist (signalling a tie) we check both candi...
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 54, u64 n <= 109,
 *            u128 n <= 217, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *dp   = NULL;   /* table of computed terms  */
    static idx_t  max_n = 0;     /* largest n for which dp is valid */

    if (n == 0) return 0;        /* undefined for n<1, but harmless */

    /* enlarge table if necessary */
    if (max_n < n) {
        idx_t new_size = n + 1;        /* indices 0..n      */
        seq_t *new_dp  = (seq_t*)realloc(dp, new_size * sizeof(seq_t));
        if (!new_dp) {                 /* allocation failure */
            return 0;
        }
        dp      = new_dp;
        /* initialise the new entries to 0 – we will overwrite them */
        for (idx_t i = max_n + 1; i <= n; ++i) dp[i] = 0;
        max_n = n;
        /* base case */
        dp[1] = 1;
    }

    /* compute missing entries up to n */
    for (idx_t k = 2; k <= n; ++k) {
        /* if already computed skip */
        if (dp[k] != 0) continue;

        /* find the largest power of two <= k/2 */
        idx_t half  = k >> 1;
        idx_t lower = 1;
        while ((lower << 1) <= half) lower <<= 1;

        /* candidate 1: lower power of two */
        seq_t prod = dp[lower] * dp[k - lower];

        /* candidate 2: the next power of two (if applicable) */
        if ((lower << 1) <= k - 1) {                    /* i <= k-1 */
            idx_t upper = lower << 1;
            if (upper < k) {                            /* i > 0 */
                seq_t prod2 = dp[upper] * dp[k - upper];
                if (prod2 > prod) prod = prod2;
            }
        }

        dp[k] = prod + 1;
    }

    return dp[n];
}
