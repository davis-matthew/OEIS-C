/*
 * A107237  Expansion of 1 / Product_{n>=0} (1 - q^(5n+2))*(1 - q^(5n+3))*(1 -
 *          q^(5n+4)).
 * https://oeis.org/A107237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 74, u32 n <= 216, u64 n <= 697,
 *            u128 n <= 2429, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107237: a(n) = number of partitions of n into parts ≡ 2,3,4 (mod 5)
   Recurrence: a(0)=1;
               a(n)=∑_{p ≤ n , p≠0,1 (mod 5)} a(n−p),  n>0
   This implementation recomputes the entire DP table for each call but
   keeps a single static array that is resized only when a larger n
   is requested.  The algorithm is O(n²) in the worst case but
   satisfies the 2‑second limit for n up to ~10 000.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static buffer for DP values, resized on demand */
    static seq_t *dp = NULL;
    static idx_t cap = 0;

    /* Resize buffer if necessary */
    if (cap < n + 1) {
        seq_t *new_dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (new_dp == NULL)
            abort();                       /* out of memory */
        dp = new_dp;
        cap = n + 1;
    }

    /* Initialize DP array for indices 0..n */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Dynamic programming over allowed parts */
    for (idx_t p = 2; p <= n; ++p) {
        /* Skip parts ≡ 0 or 1 (mod 5) */
        idx_t r = p % 5;
        if (r == 0 || r == 1)
            continue;

        /* Add contributions of part p */
        for (idx_t j = p; j <= n; ++j) {
            dp[j] += dp[j - p];
        }
    }

    return dp[n];
}
