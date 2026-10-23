/*
 * A106515  A Fibonacci-Pell convolution.
 * https://oeis.org/A106515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-3) - a(n-4), a(0)=1, a(1)=2, a(2)=6,
 *            a(3)=15
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* Ensure the table is at least 4 long for the base cases. */
    if (dp_size == 0) {
        dp_size = 4;
        dp = (seq_t *)malloc(sizeof(seq_t) * dp_size);
        /* Guard against allocation failure; if it fails, returning 0 for all n. */
        if (!dp) return (seq_t)0;
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)2;
        dp[2] = (seq_t)6;
        dp[3] = (seq_t)15;
    }

    /* If the requested index is already computed, return it. */
    if (n < dp_size) {
        return dp[n];
    }

    /* Grow the table to accommodate up to index n (inclusive). */
    idx_t new_size = n + 1;
    /* realloc is safe even if dp is non-NULL and new_size > dp_size. */
    seq_t *tmp = (seq_t *)realloc(dp, sizeof(seq_t) * (size_t)new_size);
    if (!tmp) {
        /* Allocation failed; return the last known value as a safe fallback. */
        return dp[dp_size - 1];
    }
    dp = tmp;

    /* Compute new entries using the recurrence. */
    for (idx_t i = dp_size; i <= n; ++i) {
        /* dp[i-1], dp[i-3], dp[i-4] are already known. */
        seq_t t1 = dp[i - 1];
        seq_t t2 = dp[i - 3];
        seq_t t3 = dp[i - 4];
        dp[i] = t1 * 3 - t2 * 3 - t3;
    }

    dp_size = new_size;
    return dp[n];
}
