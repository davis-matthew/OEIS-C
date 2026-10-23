/*
 * A035962  Number of partitions in parts not of the form 17k, 17k+1 or 17k-1.
 *          Also number of partitions with no part of size 1 and differences
 *          between parts at distance 7 are greater than 1.
 * https://oeis.org/A035962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of partitions of n using parts not congruent to 0, 1, or
 *            16 modulo 17
 * Range:     u8 n <= 23, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case: the sequence is defined for n >= 1.  Return 0 for n == 0 for safety. */
    if (n == 0) return (seq_t)0;

    static seq_t *dp = NULL;
    static idx_t max_n = 0;

    if (dp == NULL || n > max_n) {
        /* Allocate a new array large enough for indices 0..n. */
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        if (!new_dp) return (seq_t)0;  /* Allocation failure – return 0 as a fallback. */

        /* Initialise all entries to zero.  We treat seq_t as assignable from the integer literal 0. */
        for (idx_t i = 0; i < new_size; ++i) new_dp[i] = 0;
        new_dp[0] = 1;  /* There is exactly one way to partition 0 (the empty partition). */

        /* Dynamic programming: iterate over all allowable part sizes k <= n. */
        for (idx_t k = 2; k <= n; ++k) {
            idx_t r = k % 17;
            if (r == 0 || r == 1 || r == 16) continue;  /* Skip forbidden residues. */
            for (idx_t j = k; j <= n; ++j) {
                new_dp[j] += new_dp[j - k];
            }
        }

        /* Replace the old table with the new one. */
        free(dp);
        dp = new_dp;
        max_n = n;
    }

    return dp[n];
}
