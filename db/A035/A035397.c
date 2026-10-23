/*
 * A035397  Number of partitions of n into parts 7k+1 or 7k+3.
 * https://oeis.org/A035397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count partitions of n into parts congruent to 1 or 3 modulo 7 */

#include <stdlib.h>

static seq_t *dp = NULL;   /* dp[i] = a(i) for 0 ≤ i ≤ max_n */
static idx_t dp_size = 0;  /* allocated size of dp (indices 0..dp_size-1) */
static idx_t max_n   = 0;  /* largest n for which dp is up to date */

/* Ensure dp can hold indices up to n.  All new entries are set to 0. */
static void ensure_capacity(idx_t n)
{
    if (dp_size > n) return;

    idx_t new_size = n + 1;
    seq_t *new_dp = (seq_t *)malloc(sizeof(seq_t) * new_size);
    if (!new_dp) exit(EXIT_FAILURE);

    /* initialize new slots to zero */
    for (idx_t i = 0; i < new_size; ++i) new_dp[i] = 0;

    /* copy old values */
    if (dp) {
        for (idx_t i = 0; i < dp_size; ++i) new_dp[i] = dp[i];
        free(dp);
    }

    dp = new_dp;
    dp_size = new_size;

    /* base case */
    if (dp[0] == 0) dp[0] = 1;
}

/* Recompute dp[0..N] using the partition recurrence. */
static void recompute_dp_upto(idx_t N)
{
    if (N <= max_n) return;       /* already computed up to N */

    ensure_capacity(N);

    /* reset table */
    for (idx_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = 1;

    /* outer loop over parts */
    for (idx_t p = 1; p <= N; ++p) {
        if (p % 7 == 1u || p % 7 == 3u) {
            for (idx_t i = p; i <= N; ++i) {
                dp[i] = dp[i] + dp[i - p];
            }
        }
    }

    max_n = N;
}

/* Public function: a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n == 0 */
    recompute_dp_upto(n);
    return dp[n];
}
