/*
 * A029291  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5, 11, 12
 * Range:     u8 n <= 129, u16 n <= 904, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simple DP that rebuilds the table up to n every call.
   The table is cached in a static array that grows as needed. */

#include <stdlib.h>

static seq_t *dp = NULL;          /* DP array: dp[i] = a(i) */
static idx_t dp_size = 0;         /* allocated size of dp (>= n+1) */

/* Ensure the DP array is large enough to hold indices up to n. */
static void ensure_capacity(idx_t n)
{
    if (dp_size > n) return;                /* already big enough */

    seq_t *new_dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
    if (!new_dp) exit(1);                 /* out of memory */
    dp = new_dp;
    dp_size = n + 1;
}

/* Recompute the DP table up to index n. */
static void recompute(idx_t n)
{
    ensure_capacity(n);

    /* Initialise */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* The allowed parts */
    static const unsigned parts[] = {3, 5, 11, 12};
    const int num_parts = 4;

    /* For each part, add its contributions */
    for (int pi = 0; pi < num_parts; ++pi) {
        idx_t p = (idx_t)parts[pi];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }
}

seq_t a(idx_t n)
{
    recompute(n);
    return dp[n];
}
