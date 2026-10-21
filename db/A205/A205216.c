/*
 * A205216  Number of partitions of n into parts having no 2 in ternary
 *          representation.
 * https://oeis.org/A205216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Number of partitions of n into parts having no 2 in ternary
 *            representation
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   has_no_2_in_ternary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/has_no_2_in_ternary.h"

/* Static cache and part list */
static seq_t *dp = NULL;            /* DP table */
static size_t dp_size = 0;          /* allocated size of dp */

static idx_t *parts = NULL;         /* list of allowed parts (sorted ascending) */
static size_t parts_cap = 0;        /* capacity of parts array */
static idx_t parts_cnt = 0;         /* number of parts stored */
static idx_t parts_max_seen = 0;    /* maximum number for which parts are generated */

/* Ensure that all parts ≤ n are stored in the parts array. */
static void ensure_parts(idx_t n)
{
    if (n <= parts_max_seen)
        return;
    for (idx_t m = parts_max_seen + 1; m <= n; ++m) {
        if (has_no_2_in_ternary(m)) {
            if (parts_cnt == parts_cap) {
                size_t new_cap = parts_cap ? parts_cap * 2 : 8;
                parts = (idx_t *)realloc(parts, new_cap * sizeof(idx_t));
                parts_cap = new_cap;
            }
            parts[parts_cnt++] = m;
        }
    }
    parts_max_seen = n;
}

/* Compute the DP table up to n using the current part list. */
static void compute_dp(idx_t n)
{
    if (dp_size <= n) {
        dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        dp_size = n + 1;
    }
    /* initialise dp[0..n] */
    memset(dp, 0, dp_size * sizeof(seq_t));
    dp[0] = 1;

    for (idx_t pi = 0; pi < parts_cnt; ++pi) {
        idx_t p = parts[pi];
        if (p > n)
            break;
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    ensure_parts(n);
    compute_dp(n);
    return dp[n];
}
