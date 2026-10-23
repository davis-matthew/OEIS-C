/*
 * A045852  Number of nonnegative solutions of x1^2 + x2^2 + ... + x10^2 = n.
 * https://oeis.org/A045852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   coefficient of q^n in (1 + q + q^4 + q^9 + …)^10
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 680, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of nonnegative integer solutions of x1² + … + x10² = n */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static inline idx_t int_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    size_t len = (size_t)(n + 1);
    seq_t *dp   = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp || !next) {
        free(dp); free(next);
        return 0;
    }
    dp[0] = 1;

    /* precompute all squares ≤ n */
    idx_t max_sqrt = int_sqrt(n);
    size_t sqcnt = (size_t)(max_sqrt + 1);
    uint64_t *squares = (uint64_t *)malloc(sqcnt * sizeof(uint64_t));
    if (!squares) {
        free(dp); free(next);
        return 0;
    }
    for (size_t i = 0; i < sqcnt; ++i)
        squares[i] = (uint64_t)i * (uint64_t)i;

    /* perform 10-fold convolution */
    for (idx_t var = 0; var < 10; ++var) {
        memset(next, 0, len * sizeof(seq_t));
        for (size_t si = 0; si < sqcnt; ++si) {
            idx_t sq = (idx_t)squares[si];
            if (sq > n) break;
            for (idx_t s = sq; s <= n; ++s)
                next[s] += dp[s - sq];
        }
        /* swap dp and next */
        seq_t *tmp = dp; dp = next; next = tmp;
    }

    seq_t result = dp[n];
    free(squares);
    free(dp);
    free(next);
    return result;
}
