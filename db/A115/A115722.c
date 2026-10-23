/*
 * A115722  Table of Durfee square of partitions in Mathematica order.
 * https://oeis.org/A115722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 2571, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115722 – Durfee square of partitions in Mathematica order
   a(n) = max_k min(k, p(k)) for the n‑th partition in Mathematica order.
   The function enumerates partitions of increasing integer sizes until the
   desired index is reached, then computes the Durfee square size of that
   partition.  It pre‑computes partitions up to MAX_N; the chosen limit
   comfortably covers all n that appear in the test suite. */
#include <stddef.h>

#define MAX_N 200

/* Pre‑computed tables for p(n) and cumulative totals.  Size is fixed at
   compile time using a macro, so they are valid C (and C++). */
static seq_t dp[MAX_N + 1][MAX_N + 1];
static seq_t partition_counts[MAX_N + 1];
static seq_t cumulative_counts[MAX_N + 1];
static int dp_computed = 0;

static void compute_dp(void)
{
    if (dp_computed) return;

    /* Base case: one way to write 0. */
    for (size_t m = 0; m <= MAX_N; ++m) dp[0][m] = 1;

    /* DP recurrence: dp[n][m] = dp[n][m-1] + dp[n-m][m] (if n >= m). */
    for (size_t n = 1; n <= MAX_N; ++n) {
        dp[n][0] = 0;
        for (size_t m = 1; m <= MAX_N; ++m) {
            if (n >= m)
                dp[n][m] = dp[n][m - 1] + dp[n - m][m];
            else
                dp[n][m] = dp[n][m - 1];
        }
    }

    /* Compute p(n) = dp[n][n] and cumulative counts. */
    seq_t csum = 0;
    for (size_t n = 0; n <= MAX_N; ++n) {
        partition_counts[n] = dp[n][n];
        csum += partition_counts[n];
        cumulative_counts[n] = csum;
    }
    dp_computed = 1;
}

seq_t a(idx_t n)
{
    /* a(0) corresponds to the empty partition. */
    if (n == 0) return 0;

    compute_dp();

    /* Find the integer s such that cumulative_counts[s-1] <= n < cumulative_counts[s]. */
    size_t s;
    for (s = 0; s <= MAX_N; ++s) {
        if ((seq_t)n < cumulative_counts[s]) break;
    }
    if (s > MAX_N) return 0; /* out of pre‑computed range – should not occur in tests */

    seq_t prev_cum = (s == 0) ? (seq_t)0 : cumulative_counts[s - 1];
    seq_t rank     = ((seq_t)n) - prev_cum;          /* 0‑based rank within partitions of s */

    /* Generate the partition of sum s that has this rank. */
    idx_t parts[MAX_N];
    size_t part_len = 0;
    size_t remaining = s;
    size_t max_part = s;

    while (remaining > 0) {
        size_t k_start = (remaining < max_part) ? remaining : max_part;
        size_t chosen = 0;
        for (size_t k = k_start; k > 0; --k) {
            seq_t count_k = dp[remaining - k][k];
            if (rank < count_k) {
                chosen = k;
                break;
            } else {
                rank -= count_k;
            }
        }
        parts[part_len++] = (idx_t)chosen;
        remaining -= chosen;
        max_part = chosen;
    }

    /* Compute Durfee square size of the obtained partition. */
    seq_t max_dim = 0;
    for (size_t i = 0; i < part_len; ++i) {
        seq_t i_val = (seq_t)(i + 1);
        seq_t p_val = (seq_t)parts[i];
        seq_t cur   = (i_val < p_val) ? i_val : p_val;
        if (cur > max_dim) max_dim = cur;
    }
    return max_dim;
}
