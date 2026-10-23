/*
 * A035996  Number of partitions of n into parts not of the form 23k, 23k+8 or
 *          23k-8. Also number of partitions with at most 7 parts of size 1
 *          and differences between parts at distance 10 are greater than 1.
 * https://oeis.org/A035996
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

/* 
 * Compute A035996: Number of partitions of n into parts not of the form
 * 23k, 23k+8, or 23k-8.  Computed by dynamic programming.
 * The routine memoises the largest n seen and recomputes the table
 * from scratch whenever a larger value is requested.
 * Complexity:  O(max_n^2) in the largest n received.
 * Memory:      O(max_n) seq_t elements.
 */

#include <stdlib.h>

static seq_t *dp = NULL;          /* DP table for partition counts */
static idx_t  dp_capacity = 0;    /* Allocated size of dp */
static idx_t  max_n_computed = 0; /* Largest n for which dp is populated */

/*  Reallocate dp to at least (n+1) elements.  */
static void ensure_capacity(idx_t n)
{
    idx_t new_capacity = n + 1;
    if (dp_capacity >= new_capacity) return;

    seq_t *tmp = (seq_t *)realloc(dp, new_capacity * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);  // abort on allocation failure
    dp = tmp;
    dp_capacity = new_capacity;
}

/*  Fill dp[0..n] with partition numbers for the current n.  */
static void extend_to_n(idx_t n)
{
    if (n <= max_n_computed) return;

    ensure_capacity(n);

    /* initialise dp[0]=1 and the rest to 0 */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* dynamic programming over allowed parts */
    for (idx_t p = 1; p <= n; ++p) {
        idx_t m = p % 23;
        if (m == 0 || m == 8 || m == 15) continue;   /* forbidden */
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    max_n_computed = n;
}

/*  Public interface requested by the test harness.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1 */
    extend_to_n(n);
    return dp[n];
}
