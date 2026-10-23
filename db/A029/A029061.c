/*
 * A029061  Expansion of 1/((1-x)*(1-x^3)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 508, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029061: Number of partitions of n into parts 1, 3, 10 and 12.
   The function uses a classic dynamic‑programming recurrence with an
   inner loop over the allowed part sizes; this counts unordered
   partitions correctly.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for the DP table */
    static seq_t *dp   = NULL;   /* dp[i] = number of partitions of i */
    static idx_t  size = 0;      /* allocated size of dp */

    /* ensure we have room for indices up to n */
    if (size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* avoid undefined behaviour */
        dp = tmp;
        size = new_size;
    }

    /* reset the table up to n */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                      /* one way to partition 0 */

    /* allowed part sizes */
    static const idx_t parts[4] = {1, 3, 10, 12};

    /* dynamic programming: for each part, add its contributions */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }
    return dp[n];
}
