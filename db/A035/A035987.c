/*
 * A035987  Number of partitions of n into parts not of the form 21k, 21k+9 or
 *          21k-9. Also number of partitions with at most 8 parts of size 1
 *          and differences between parts at distance 9 are greater than 1.
 * https://oeis.org/A035987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of partitions of n into parts not of the form 21k, 21k+9, 21k-9.
   Computed by dynamic programming over all allowed part sizes.
   Recurrence: dp[0] = 1;
               for each allowed part p (1 ≤ p ≤ n):
                   for j = p to n:
                       dp[j] += dp[j - p];
   dp[j] thus counts unrestricted partitions using only the allowed parts.
   The table is recomputed whenever a larger n is requested.
   (No memoisation of intermediate results beyond the maximal n seen.)   */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Static table of partition counts.  dp_size = table size (maximum n + 1). */
static seq_t *dp = NULL;
static idx_t dp_size = 0;

/* Recompute the DP table up to value `n`.  After this call, dp[n] contains
   a(n) and the table covers all indices ≤ n. */
static void compute_up_to(idx_t n)
{
    /* Allocate new table and zero it. */
    seq_t *new_dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!new_dp) return;                /* allocation failure: undefined behaviour */
    memset(new_dp, 0, (size_t)(n + 1) * sizeof(seq_t));

    new_dp[0] = (seq_t)1;               /* base case: one way to partition 0 */

    /* Dynamic‑programming loop over allowed parts. */
    for (idx_t p = 1; p <= n; ++p) {
        /* Skip forbidden part sizes: residues 0, 9, or 12 modulo 21. */
        idx_t mod = p % 21;
        if (mod == 0 || mod == 9 || mod == 12) continue;

        for (idx_t j = p; j <= n; ++j) {
            new_dp[j] = new_dp[j] + new_dp[j - p];
        }
    }

    /* Replace the old table with the new one. */
    free(dp);
    dp      = new_dp;
    dp_size = n + 1;   /* dp covers indices 0 … n */
}

/* Public function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1, a(1) is the first term */

    /* Ensure the table is up to date. */
    if (dp_size <= n) compute_up_to(n);

    return dp[n];
}
