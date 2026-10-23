/*
 * A103265  Number of partitions of n in which both even and odd square parts
 *          occur in 2 forms c, c* and with multiplicity 1. There is no
 *          restriction on parts which are twice squares.
 * https://oeis.org/A103265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1+x^{k^2})/(1-x^{k^2})
 * Range:     u8 n <= 35, u16 n <= 129, u32 n <= 577, u64 n <= 3026,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* special case n == 0 */
    if (n == 0) return 1;

    /* size for array indexing */
    size_t N = (size_t)n;

    /* allocate arrays */
    seq_t *dp      = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *sum     = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp || !dp_next || !sum) {
        /* allocation failure – return 0 as a fallback */
        return 0;
    }

    dp[0] = 1;   /* base: one empty partition */

    /* iterate over square sizes */
    for (idx_t k = 1;; ++k) {
        idx_t sq = k * k;
        if (sq > n) break;

        /* compute cumulative sums of dp for this square */
        for (size_t i = 0; i <= N; ++i) {
            if (i >= (size_t)sq) {
                sum[i] = sum[i - sq] + dp[i - sq];
            } else {
                sum[i] = 0;
            }
        }

        /* update dp with the current square's contribution */
        for (size_t i = 0; i <= N; ++i) {
            seq_t twice = sum[i] + sum[i];   /* 2 * sum[i] */
            dp_next[i] = dp[i] + twice;
        }

        /* prepare for next iteration */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    seq_t result = dp[N];

    free(sum);
    free(dp_next);
    free(dp);

    return result;
}
