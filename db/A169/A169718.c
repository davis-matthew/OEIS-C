/*
 * A169718  Number of ways of making change for n cents using coins of 1, 5,
 *          10, 25, 50 and 100 cents.
 * https://oeis.org/A169718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of ways of making change for n cents using coins of 1, 5,
 *            10, 25, 50 and 100 cents. Computed by dynamic programming on the
 *            partition function for these denominations
 * Range:     u8 n <= 99, u16 n <= 454, u32 n <= 4934, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const idx_t COINS[6] = {1, 5, 10, 25, 50, 100};

seq_t a(idx_t n)
{
    /* Keep a static buffer large enough for the largest n requested
       so that reallocations are infrequent.  The buffer holds values
       up to n, indexed exactly as the OEIS sequence. */
    static seq_t *dp       = NULL;
    static size_t  dp_cap  = 0;

    /* Grow the buffer if necessary.  Newly allocated entries are
       initialized to zero. */
    if (dp_cap < n + 1) {
        size_t new_cap = n + 1;
        seq_t *new_dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!new_dp) abort();
        /* If the buffer increased, zero the new part. */
        if (new_cap > dp_cap)
            memset(new_dp + dp_cap, 0, (new_cap - dp_cap) * sizeof(seq_t));
        dp     = new_dp;
        dp_cap = new_cap;
    }

    /* Reset dp[0..n] to zero except dp[0] = 1.  We could keep the
       values from older calls to avoid clearing, but recomputing all
       entries is inexpensive for n ≤ 10 000. */
    for (idx_t i = 0; i <= n; ++i) dp[i] = (idx_t)i == 0 ? (seq_t)1 : (seq_t)0;

    /* For each coin type, add its contribution to all larger amounts. */
    for (int k = 0; k < 6; ++k) {
        idx_t c = COINS[k];
        for (idx_t i = c; i <= n; ++i) {
            dp[i] += dp[i - c];
        }
    }

    return dp[n];
}
