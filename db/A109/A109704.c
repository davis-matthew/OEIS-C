/*
 * A109704  Number of partitions of n into parts each equal to 2 mod 7.
 * https://oeis.org/A109704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 285, u32 n <= 844, u64 n <= 2780,
 *            u128 n <= 9865, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the number of partitions of n into parts each congruent to 2 modulo 7.
   Uses a straightforward dynamic‑programming (coin‑change) algorithm:
   a(0) = 1, a(n) = Σ a(n - k) for all k ∈ {2,9,16,…} ≤ n.
   The DP array is recalculated completely only when a larger index is requested.
   This guarantees correctness for all n and avoids double counting.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;      /* array of a(i) for 0 ≤ i ≤ current_max */
    static idx_t current_max = 0; /* largest n that has been fully computed */

    /* Allocate or resize the array to accommodate n */
    if (dp == NULL || n > current_max) {
        idx_t new_len = n + 1;
        dp = (seq_t *)realloc(dp, new_len * sizeof(seq_t));
        /* Initialise all entries to 0 */
        for (idx_t i = 0; i < new_len; ++i)
            dp[i] = 0;
        dp[0] = 1; /* a(0) = 1 */

        /* Complete the DP for all i ≤ n */
        for (idx_t coin = 2; coin <= n; coin += 7) {
            for (idx_t i = coin; i <= n; ++i) {
                dp[i] += dp[i - coin];
            }
        }
        current_max = n;
    }

    return dp[n];
}
