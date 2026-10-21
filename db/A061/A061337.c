/*
 * A061337  Smallest number of distinct triangular numbers which sum to n (or
 *          -1 if not possible).
 * https://oeis.org/A061337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 121, i16 n <= 122, i32 n <= 122, i64 n <= 122,
 *            i128 n <= 122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A061337: the smallest number of distinct triangular
   numbers whose sum equals n (or -1 if none).  Runs a 0/1‑knapsack DP
   over triangular numbers t_k = k*(k+1)/2.  Complexity O(n·√n). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *dp = NULL;       /* DP table: dp[s] = minimal # of terms for sum s */
static idx_t cached_max = 0;   /* largest n for which dp has been built */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Re‑build DP when we need a larger n than we have already computed. */
    if (dp == NULL || n > cached_max) {
        size_t size = (size_t)(n + 1);
        seq_t *new_dp = (seq_t *)realloc(dp, size * sizeof(seq_t));
        if (new_dp == NULL) {
            return (seq_t)-1;   /* memory allocation failed */
        }
        dp = new_dp;

        /* Initialise the table.  -1 means “unreachable”. */
        dp[0] = 0;
        for (idx_t i = 1; i <= n; ++i) {
            dp[i] = (seq_t)-1;
        }

        /* 0/1 knapsack: add each triangular number once. */
        for (idx_t k = 1; ; ++k) {
            seq_t t = ((seq_t)k * ((seq_t)k + 1)) / 2;
            if (t > (seq_t)n) break;
            idx_t t_idx = (idx_t)t;
            for (idx_t s = n; s >= t_idx; --s) {
                if (dp[s - t_idx] != (seq_t)-1) {
                    seq_t cand = dp[s - t_idx] + 1;
                    if (dp[s] == (seq_t)-1 || cand < dp[s]) {
                        dp[s] = cand;
                    }
                }
            }
        }

        cached_max = n;
    }

    return dp[n];
}
