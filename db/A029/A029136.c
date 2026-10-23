/*
 * A029136  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^4)*(1-x^6)).
 * https://oeis.org/A029136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 375, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029136
   Number of partitions of n into parts 2, 3, 4, and 6.
   Computed by a coin‑change dynamic programming routine. */
#include <stdlib.h>

static seq_t *dp = NULL;                 /* array of DP values */
static idx_t  dp_cap = 0;                /* allocated capacity (number of elements) */
static idx_t  dp_max = (idx_t)-1;        /* highest n for which dp[0..dp_max] is valid */

/* Returns the n‑th term of OEIS A029136. */
seq_t a(idx_t n)
{
    /* Allocate or grow the DP array if necessary. */
    if (n + 1 > dp_cap) {
        seq_t *tmp = (seq_t *)realloc(dp,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        dp = tmp;
        dp_cap = n + 1;
    }

    /* Recompute the DP table if n is larger than the previously computed range
       or if this is the first call (dp_max == (idx_t)-1). */
    if (dp_max == (idx_t)-1 || n > dp_max) {
        /* Initialise to zero up to n. */
        for (idx_t i = 0; i <= n; ++i)
            dp[i] = 0;

        dp[0] = 1;   /* one way to partition 0: use no parts */

        /* Compute partitions using the parts 2, 3, 4, and 6. */
        const idx_t parts[4] = { 2, 3, 4, 6 };
        for (int p = 0; p < 4; ++p) {
            idx_t part = parts[p];
            for (idx_t i = part; i <= n; ++i)
                dp[i] += dp[i - part];
        }
        dp_max = n;
    }

    return dp[n];
}
