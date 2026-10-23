/*
 * A029222  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 6, 9, 11
 * Range:     u8 n <= 107, u16 n <= 761, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n‑th term of the sequence */
seq_t a(idx_t n)
{
    /* Allocate a DP table of size n+1, all zeroed */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;                /* allocation failed */

    dp[0] = 1;                         /* base case */

    /* Allowed summands */
    const idx_t parts[4] = { 2, 6, 9, 11 };

    /* Standard unbounded‑knapsack (partition) DP:
       For each part, add its contribution to all reachable sums. */
    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
