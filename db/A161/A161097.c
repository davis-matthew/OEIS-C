/*
 * A161097  Number of partitions of n into squares where every part appears at
 *          least 9 times.
 * https://oeis.org/A161097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of n into squares where every part
 *            appears at least 9 times
 * Range:     u8 n <= 227, u16 n <= 704, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The sequence is computed by dynamic programming on the generating function
   G(x) = Π_{k≥1} (1 + x^(9*k^2) / (1 - x^(k^2)))
   For each square s = k^2 we add the contribution of all multiples m*s (m ≥ 9). */
seq_t a(idx_t n)
{
    /* Allocate the DP array (size n+1) */
    seq_t *dp = (seq_t *)calloc((size_t)(n) + 1, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* fallback, should not happen */

    /* Base case: empty partition */
    dp[0] = (seq_t)1;

    /* Temporary array to hold a copy of dp from the previous iteration */
    seq_t *dp_old = (seq_t *)calloc((size_t)(n) + 1, sizeof(seq_t));
    if (!dp_old) {
        free(dp);
        return (seq_t)0;
    }

    /* Process each square s = k^2 while 9*s <= n */
    for (idx_t k = 1; ; ++k) {
        idx_t s = k * k;                 /* square weight */
        idx_t min_total = 9 * s;
        if (min_total > n)
            break;                       /* no more squares can be used */

        /* Copy current dp into dp_old to use as the previous state */
        for (idx_t i = 0; i <= n; ++i)
            dp_old[i] = dp[i];

        /* Update dp using the contribution of square s */
        for (idx_t r = 0; r < s && r <= n; ++r) {
            seq_t sumAll   = 0;          /* sum of all dp_old values so far in this modulo class */
            seq_t lastSum  = 0;          /* sum of the last up to 8 values (distance < 9*s) */
            seq_t buffer[8];             /* circular buffer of the last 8 values */
            size_t buf_cnt = 0;          /* number of values processed in this modulo class */

            for (idx_t i = r; i <= n; i += s) {
                /* Sum of dp_old values with distance ≥ 9*s from i */
                seq_t eligibleSum = sumAll - lastSum;
                dp[i] = dp_old[i] + eligibleSum;

                seq_t val = dp_old[i];
                if (buf_cnt < 8) {
                    buffer[buf_cnt] = val;
                    lastSum += val;
                } else {
                    size_t idx = buf_cnt % 8;
                    seq_t old = buffer[idx];
                    lastSum -= old;
                    buffer[idx] = val;
                    lastSum += val;
                }
                sumAll += val;
                ++buf_cnt;
            }
        }
    }

    seq_t ans = dp[n];
    free(dp);
    free(dp_old);
    return ans;
}
