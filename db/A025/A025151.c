/*
 * A025151  Number of partitions of n into distinct parts >= 6.
 * https://oeis.org/A025151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 112, u32 n <= 296, u64 n <= 898,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025151: Number of partitions of n into distinct parts >= 6.
   Formula: a(n) = coefficient of x^n in ∏_{j≥6}(1 + x^j). */

#include <stdlib.h>
#include <string.h>

static seq_t *dp = NULL;          /* temporary DP array */
static idx_t dp_cap = 0;          /* current capacity (number of elements) */

/* Compute a(n).  Reuse the DP array across calls for efficiency. */
seq_t a(idx_t n)
{
    /* Grow the DP array if necessary. */
    if (dp_cap <= n) {
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        dp = tmp;
        dp_cap = n + 1;
    }

    /* Clear entries up to n. */
    memset(dp, 0, (n + 1) * sizeof(seq_t));
    dp[0] = 1;  /* empty partition */

    /* Standard 0/1 knapsack DP for distinct parts. */
    for (idx_t part = 6; part <= n; ++part) {
        for (idx_t sum = n; sum >= part; --sum) {
            dp[sum] += dp[sum - part];
        }
    }

    return dp[n];
}
