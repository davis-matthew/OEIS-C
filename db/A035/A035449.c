/*
 * A035449  Number of partitions of n into parts 8k+1 or 8k+3.
 * https://oeis.org/A035449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 133, u32 n <= 425, u64 n <= 1475,
 *            u128 n <= 5403, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A035449
    Number of partitions of n into parts congruent to 1 or 3 modulo 8.
    The sequence is computed by a classic coin‑change dynamic programming
    routine.  For each admissible part p the recurrence
        dp[j] += dp[j-p]   (j >= p)
    is applied.  The table dp[0..max_n] is regenerated from scratch whenever
    the requested index n exceeds the current maximum, which keeps the
    implementation simple and guarantees correctness for arbitrary call
    order and values of n.  The algorithm runs in O(max_n^2/4) time,
    which is easily below the 2‑second limit for n ≤ 10000.
*/
#include <stdlib.h>

/* Global state – the partition table and the size of that table. */
static seq_t *dp = NULL;
static idx_t current_max = 0;   /* largest n for which dp[0..current_max] is valid */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* Sequence is defined for n ≥ 1; return 0 for safety. */

    if (n > current_max) {
        /* Allocate a new table of the required size. */
        seq_t *new_dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!new_dp)   /* Allocation failed – unlikely but safe. */
            return 0;
        dp = new_dp;
        current_max = n;

        /* Initialise the table. */
        for (idx_t i = 0; i <= n; ++i)
            dp[i] = 0;
        dp[0] = 1;  /* Empty partition. */

        /* Classic coin‑change DP over all admissible parts ≤ n. */
        for (idx_t part = 1; part <= n; ++part) {
            /* Select parts ≡ 1 or 3 (mod 8). */
            if ((part & 7) != 1 && (part & 7) != 3)
                continue;
            for (idx_t j = part; j <= n; ++j)
                dp[j] += dp[j - part];
        }
    }

    return dp[n];
}
