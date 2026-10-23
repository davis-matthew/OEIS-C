/*
 * A029279  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A029279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into 3,5,7,10 (coin‑change DP –
 *            unordered partitions)
 * Range:     u8 n <= 104, u16 n <= 732, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* array for all values from 0 to n inclusive */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                 /* allocation failed */

    dp[0] = (seq_t)1;                   /* one way to make sum 0 */

    const int parts[4] = {3, 5, 7, 10};

    /* For each part add ways from lower sums */
    for (int pi = 0; pi < 4; ++pi) {
        int p = parts[pi];
        for (size_t i = (size_t)p; i < len; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[len - 1];
    free(dp);
    return result;
}
