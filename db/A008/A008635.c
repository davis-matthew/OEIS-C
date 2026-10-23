/*
 * A008635  Molien series for alternating group Alt_12 (or A_12).
 * https://oeis.org/A008635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p12(n) + p12(n-66) where p12(k) is the number of
 *            partitions * of k with all parts ≤ 12. p12 is obtained from the
 *            generating function * 1/((1-x)(1-x²)…(1-x¹²)) by dynamic
 *            programming
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 188, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate array dp[0..n] for partition counts */
    size_t len = (size_t)(n + 1);
    seq_t *dp = (seq_t*)calloc(len, sizeof(seq_t));
    if (!dp) return 0;          /* memory failure – return 0 as safe value */

    dp[0] = 1;                  /* one way to make sum 0: use no parts */

    for (idx_t k = 1; k <= 12; ++k) {
        for (idx_t i = k; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - k];
        }
    }

    seq_t res = dp[n];
    if (n >= 66) {
        res = res + dp[n - 66];
    }

    free(dp);
    return res;
}
