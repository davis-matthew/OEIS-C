/*
 * A036010  Number of partitions of n into parts not of the form 25k, 25k+11
 *          or 25k-11. Also number of partitions with at most 10 parts of size
 *          1 and differences between parts at distance 11 are greater than 1.
 * https://oeis.org/A036010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A036010: Number of partitions of n into parts not of the form 25k,
    25k+11 or 25k-11.  The algorithm counts partitions using a coin‐change
    dynamic programming approach.  For each allowed part size k we add to
    every dp[j] for j ≥ k the value dp[j‑k].  This requires ~0.44 n²
    additions, which is well below the 2 s limit for n ≤ 10000. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* All indices used in loops are capped by the value of n, which
       fits easily in a size_t because n ≤ 10000 in the tests. */
    size_t N = (size_t)n;
    /* Allocate dp[0..N] and zero‑initialise it. */
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* allocation failure – return 0 */
    dp[0] = (seq_t)1;                  /* empty partition */

    for (size_t k = 1; k <= N; ++k) {
        /* Skip forbidden residues modulo 25: 0, 11, 14. */
        if (k % 25 == 0 || k % 25 == 11 || k % 25 == 14)
            continue;
        for (size_t j = k; j <= N; ++j)
            dp[j] += dp[j - k];
    }
    seq_t ans = dp[N];
    free(dp);
    return ans;
}
