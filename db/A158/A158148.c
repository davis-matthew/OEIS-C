/*
 * A158148  Number of nondecreasing integer sequences of length 14 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{r=1}^{13} p(n,r) * Σ_{u=1}^{14-r} p(n,u) * where
 *            p(n,k) is the number of integer partitions of n into exactly k
 *            parts. * * The sequence counts nondecreasing integer sequences
 *            of length 14 with sum 0 * and sum of absolute values 2n.
 *            Partition counts are computed via the * recurrence for...
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 59, u64 n <= 433, u128 n <= 433,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n may be zero but the function is only evaluated for n >= 1. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* The maximum number of parts needed is 13 (ell-1 for length 14). */
    const size_t K = 13;
    const size_t N = (size_t)n;           /* convert to native size_t for loops */

    /* Allocate a 2‑D DP table dp[j][s] (j = number of parts, s = sum).
       dp[j][s] = number of partitions of s into parts not exceeding j. */
    size_t rows = K + 1;
    size_t cols = N + 1;
    seq_t *dp = (seq_t *)calloc(rows * cols, sizeof(seq_t));
    if (!dp) return (seq_t)0;            /* allocation failed – return 0 */

    /* Helper to index the flattened array. */
    #define IDX(j, s) ((j) * cols + (s))

    /* Base: one partition of 0 using 0 parts. */
    dp[IDX(0, 0)] = (seq_t)1;

    /* Fill DP table. */
    for (size_t j = 1; j <= K; j++) {
        for (size_t s = 0; s <= N; s++) {
            seq_t val = dp[IDX(j - 1, s)];
            if (s >= j) {
                val += dp[IDX(j, s - j)];
            }
            dp[IDX(j, s)] = val;
        }
    }

    /* Compute p[n][k] = partitions of n into exactly k parts.
       For n >= k, p[n][k] = dp[k][n - k] (subtract 1 from each part). */
    seq_t part[14] = {0};        /* indices 0..13; part[0] unused */
    for (size_t k = 1; k <= K; k++) {
        if (n >= k) {
            part[k] = dp[IDX(k, N - k)];
        } else {
            part[k] = (seq_t)0;
        }
    }

    /* Prefix sums of part[k] for k = 1 .. 13. */
    seq_t prefix[14] = {0};
    for (size_t k = 1; k <= K; k++) {
        prefix[k] = prefix[k - 1] + part[k];
    }

    /* Final summation over r and u. */
    seq_t result = (seq_t)0;
    for (size_t r = 1; r <= K; r++) {
        size_t u_max = 14 - r;          /* u is at least 1, at most u_max */
        if (u_max == 0) continue;       /* should not happen */
        seq_t sum_u = prefix[u_max];
        result += part[r] * sum_u;
    }

    free(dp);
    return result;
}
