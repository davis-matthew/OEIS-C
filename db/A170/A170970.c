/*
 * A170970  Expansion of Product_{i=0..m-1} (1 + x^(4*i+1)) for m = 7.
 * https://oeis.org/A170970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of subsets of {1,5,9,13,17,21,25} summing to n
 *            Initial value: a(0) = 1 This implements the DP for the
 *            coefficient of the expansion of ∏_{i=0..6} (1 + x^(4*i+1)). The
 *            dynamic programming array is pre‑computed up to the largest
 *            possible sum (91). For any n > 91 the result is 0
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* maximum sum of the exponents is 1+5+9+13+17+21+25 = 91 */
    static const idx_t exps[7] = {1,5,9,13,17,21,25};
    static seq_t dp[92];
    static int initialized = 0;

    if (!initialized) {
        /* initialize all entries to 0 */
        for (size_t i = 0; i < 92; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;          /* empty subset */

        for (size_t k = 0; k < 7; ++k) {
            idx_t e = exps[k];
            /* iterate backwards to avoid reuse of same element */
            for (int i = (int)91; i >= (int)e; --i) {
                dp[i] += dp[i - e];
            }
        }
        initialized = 1;
    }

    if (n < 92) return dp[(size_t)n];
    return (seq_t)0;   /* sums larger than 91 cannot be achieved */
}
