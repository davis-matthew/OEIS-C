/*
 * A035958  Number of partitions of n into parts not of the form 15k, 15k+4 or
 *          15k-4. Also number of partitions with at most 3 parts of size 1
 *          and differences between parts at distance 6 are greater than 1.
 * https://oeis.org/A035958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035958 – Number of partitions of n into parts not of the form
   15k, 15k+4 or 15k+11.  The routine uses a simple DP that is
   recomputed whenever the requested n exceeds the previously
   computed maximum.  Complexity is Θ(n²) for the largest n
   requested (≈10⁸ elementary arithmetic operations for n = 10⁴). */

#include <stdlib.h>

static seq_t *dp = NULL;   /* dynamic array: dp[i] = required number of partitions */
static idx_t dp_len = 0;   /* current allocated size (dp_len = max_n + 1) */

/* Recompute the DP table up to the given index n (inclusive). */
static void recompute_up_to(idx_t n)
{
    /* Allocate or resize the DP array. */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(dp, new_len * sizeof(seq_t));
    if (!tmp) exit(1);
    dp = tmp;
    dp_len = new_len;

    /* Initialize the table. */
    dp[0] = 1;                     /* one way to partition zero */
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Standard integer‑partition DP: process allowed part sizes
       in increasing order. */
    for (idx_t p = 1; p <= n; ++p) {
        if (p % 15 == 0 || p % 15 == 4 || p % 15 == 11) continue; /* forbidden */
        for (idx_t m = p; m <= n; ++m) {
            dp[m] += dp[m - p];
        }
    }
}

/* External function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* sequence is defined for n ≥ 1 */
    if (n >= dp_len) {
        recompute_up_to(n);
    }
    return dp[n];
}
