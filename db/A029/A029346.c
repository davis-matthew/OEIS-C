/*
 * A029346  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A029346: Number of partitions of n into parts 4, 5, 11, and 12.
   This implementation builds the DP array from scratch for each call,
   adding each part in turn to preserve the order‑independent counting.
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Array of allowed part sizes */
    static const idx_t parts[4] = {4, 5, 11, 12};

    /* Allocate space for DP[0..n] */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) exit(1);            /* allocation failure */

    dp[0] = 1;                    /* empty partition */

    /* For each part size, update all sums that include this part.
       The outer part loop guarantees that each unordered partition
       is counted exactly once. */
    for (int k = 0; k < 4; ++k) {
        idx_t p = parts[k];
        for (idx_t s = p; s <= n; ++s) {
            dp[s] += dp[s - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
