/*
 * A224366  Number of compositions of n^2 into sums of squares.
 * https://oeis.org/A224366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n² into sums of squares
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t m = (idx_t)n * (idx_t)n;          /* m = n^2 */
    if (m == 0) return (seq_t)1;           /* a(0) = 1 */

    /* Allocate array dp[0..m] of seq_t values. */
    seq_t *dp = (seq_t*)malloc((size_t)(m + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;              /* allocation failed (unlikely) */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= m; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k * k <= i; ++k) {
            sum += dp[i - k * k];
        }
        dp[i] = sum;
    }

    seq_t result = dp[m];
    free(dp);
    return result;
}
