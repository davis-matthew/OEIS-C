/*
 * A029289  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5, 10, 11
 * Range:     u8 n <= 122, u16 n <= 851, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate array for indices 0 … n */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) return 0;               /* allocation error, unlikely for tests */

    /* initialise */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i < len; ++i) dp[i] = (seq_t)0;

    /* allowed parts */
    static const size_t parts[] = {3, 5, 10, 11};
    const size_t parts_cnt = sizeof(parts) / sizeof(parts[0]);

    /* dynamic programming */
    for (size_t k = 0; k < parts_cnt; ++k) {
        size_t p = parts[k];
        for (size_t i = p; i < len; ++i) {
            dp[i] = dp[i] + dp[i - p];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
