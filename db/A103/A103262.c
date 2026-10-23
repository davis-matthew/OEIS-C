/*
 * A103262  McKay-Thompson series of class 36g for the Monster group.
 * https://oeis.org/A103262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts not
 *            divisible by 3, each part having two distinct types. The
 *            generating function is ∏_{k>0,3∤k} (1 + 2x^k + x^{2k})
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 169, u64 n <= 578,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* DP array: dp[s] = #partitions of sum s */
static idx_t  dp_cap = 0;     /* current allocated size */

/* Ensure dp has room for indices 0..n */
static void ensure_dp_size(idx_t n)
{
    idx_t needed = n + 1;
    if (needed <= dp_cap)
        return;

    seq_t *new_dp = (seq_t *)realloc(dp, needed * sizeof(seq_t));
    if (!new_dp) {
        /* allocation failure: terminate program */
        exit(EXIT_FAILURE);
    }
    dp = new_dp;
    dp_cap = needed;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    ensure_dp_size(n);

    /* base case */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* process each distinct part weight not divisible by 3 */
    for (idx_t part = 1; part <= n; ++part) {
        if (part % 3 == 0)
            continue;

        /* each part has two distinct types → two separate items */
        for (int copy = 0; copy < 2; ++copy) {
            for (idx_t s = n; s >= part; --s) {
                dp[s] += dp[s - part];
            }
        }
    }

    return dp[n];
}
