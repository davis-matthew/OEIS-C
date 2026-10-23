/*
 * A029318  Expansion of 1/((1-x^3)*(1-x^8)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 134, u16 n <= 983, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A029318: number of partitions of n into parts 3, 8, 9 and 12. */
/* Recurrence: a(0)=1, a(n)=a(n‑3)+a(n‑8)+a(n‑9)+a(n‑23)+a(n‑24)+a(n‑29) */
/*            –(a(n‑11)+a(n‑15)+a(n‑17)+a(n‑21)+a(n‑32)) for n>0. */
/*   all a(k)=0 for k<0. */

#include <stdlib.h>
/* idx_t and seq_t are supplied by the build system.  No other headers are required. */

seq_t a(idx_t n)
{
    /* Zero‑initialize the DP array with calloc so that all a(k<0)=0. */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* out of memory – return 0 */

    dp[0] = (seq_t)1;                  /* base case */

    /* Pre‑incrementally fill dp[1..n] */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t pos_sum = 0, neg_sum = 0;

        /* Positive contributions */
        if (i >= (idx_t)3) pos_sum += dp[i - 3];
        if (i >= (idx_t)8) pos_sum += dp[i - 8];
        if (i >= (idx_t)9) pos_sum += dp[i - 9];
        if (i >= (idx_t)23) pos_sum += dp[i - 23];
        if (i >= (idx_t)24) pos_sum += dp[i - 24];
        if (i >= (idx_t)29) pos_sum += dp[i - 29];

        /* Negative contributions */
        if (i >= (idx_t)11) neg_sum += dp[i - 11];
        if (i >= (idx_t)15) neg_sum += dp[i - 15];
        if (i >= (idx_t)17) neg_sum += dp[i - 17];
        if (i >= (idx_t)21) neg_sum += dp[i - 21];
        if (i >= (idx_t)32) neg_sum += dp[i - 32];

        /* Since pos_sum ≥ neg_sum for partitions numbers, subtraction won't underflow. */
        dp[i] = pos_sum - neg_sum;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
