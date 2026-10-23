/*
 * A106206  Coefficients of (1 + 144*x)^(1/24).
 * https://oeis.org/A106206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies the recurrence * a(0) = 1 * n*a(n) +
 *            6*(24*n-25)*a(n-1) = 0 for n ≥ 1 * (Coefficients of (1 +
 *            144*x)^(1/24))
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 9, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;
static idx_t dp_size = 0;

seq_t a(idx_t n)
{
    /* Initialise storage for the first term. */
    if (dp_size == 0) {
        dp_size = 1;
        dp = (seq_t *)realloc(dp, dp_size * sizeof(seq_t));
        dp[0] = 1;
    }
    /* Grow the memoisation array if needed. */
    if (dp_size <= n) {
        idx_t new_size = n + 1;
        dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        dp_size = new_size;
    }
    /* Compute terms up to n. */
    for (idx_t i = dp[dp_size - 1] == 0 ? dp_size - 1 : dp_size - 1; i <= n; ++i) {
        if (i == 0) continue; /* already set */
        seq_t prev = dp[i - 1];
        /* Compute the numerator: -6 * (24*i - 25). */
        seq_t num = -6 * ((seq_t)(24 * (seq_t)i - 25));
        seq_t denom = (seq_t)i;
        /* Apply the recurrence: a(i) = prev * num / denom. */
        dp[i] = (prev * num) / denom;
    }
    return dp[n];
}
