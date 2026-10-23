/*
 * A029364  Expansion of 1/((1-x^4)*(1-x^7)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029364: Expansion of 1/((1-x^4)*(1-x^7)*(1-x^9)*(1-x^11))
   Number of partitions of n into parts 4, 7, 9, and 11.
   The value is computed by the classic unbounded‑knapsack dynamic
   programming algorithm.  For each n the table up to n is built from
   scratch; this guarantees correctness in any call order. */

#include <stdlib.h>   /* malloc, free */

static const idx_t PARTS[4] = {4, 7, 9, 11};

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Allocate a table for all values 0..n */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* allocation failure */

    /* Initialise */
    dp[0] = (seq_t)1;                  /* one way to make 0 */
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Classic unbounded knapsack DP */
    for (idx_t part_idx = 0; part_idx < 4; ++part_idx) {
        idx_t part = PARTS[part_idx];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
