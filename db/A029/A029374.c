/*
 * A029374  Expansion of 1/((1-x^4)*(1-x^9)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029374: partitions of n into parts 4, 9, 10 and 11
   computed by the classic unbounded coin‑change dynamic programming.
   The function is O(n) in time and O(n) in memory, and works for
   any of the predefined integer or arbitrary‑precision types. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static const idx_t parts[] = {4, 9, 10, 11};
    const size_t P = sizeof(parts) / sizeof(parts[0]);

    /* Allocate array for the DP table. Use (seq_t *)cast to satisfy
       both C and C++ compiler rules. */
    seq_t *dp = (seq_t *)calloc((n + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;        /* allocation failure – unlikely */

    dp[0] = (seq_t)1;                 /* only one way to make sum 0 */

    /* Iterate over each coin denomination, adding ways to make sums
       that include that coin. Unbounded usage is allowed. */
    for (size_t pi = 0; pi < P; ++pi)
    {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    seq_t ans = dp[n];
    free(dp);
    return ans;
}
