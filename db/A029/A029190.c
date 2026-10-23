/*
 * A029190  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   partitions of n into parts 2,4,9,12 (coin‑change DP)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static buffer for the DP array */
    static seq_t *dp  = NULL;
    static size_t cap = 0;
    const idx_t parts[4] = {2, 4, 9, 12};

    /* grow the buffer if necessary */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > cap) {
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        dp  = tmp;
        cap = new_cap;
    }

    /* initialise dp[0] = 1, all other entries 0 up to n */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* classic unbounded‑knapsack / coin‑change DP:
       for every part, add its contribution forward, allowing reuse of parts */
    for (size_t p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        if (part > n) continue;
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    return dp[n];
}
