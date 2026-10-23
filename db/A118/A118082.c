/*
 * A118082  Number of partitions of n such that largest part k occurs
 *          floor(k/2) times.
 * https://oeis.org/A118082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n such that largest part k occurs
 *            floor(k/2) times. Computation uses the generating function a(n)
 *            = sum_{k>=1} p_{k-1}(n - k*floor(k/2)), where p_m(s) is the
 *            number of partitions of s into parts ≤ m
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Forward declaration of helper to rebuild the partition table. */
static void build_dp(idx_t max_n);

/* Partition table: dp[s*(dp_k+1)+j] = number of partitions of s into parts ≤ j. */
static seq_t *dp = NULL;   /* flattened 2-d array                     */
static idx_t  dp_n = 0;    /* current maximum n stored in dp          */
static idx_t  dp_k = 0;    /* current maximum largest part in dp      */

static void
build_dp(idx_t max_n)
{
    /* Determine appropriate size for the largest part.
       k*floor(k/2) <= max_n implies k <= sqrt(2*max_n).  We add 2 for safety. */
    idx_t max_k = (idx_t)(sqrt((long double)max_n * 2.0L)) + 2;
    size_t total = (size_t)(max_n + 1) * (max_k + 1);

    seq_t *new_dp = (seq_t *)realloc(dp, total * sizeof(seq_t));
    if (!new_dp) abort();   /* out of memory */
    dp = new_dp;
    dp_n = max_n;
    dp_k = max_k;

    /* Initialize: dp[0][j] = 1 for all j; dp[s][0] = 0 for s>0. */
    for (idx_t j = 0; j <= max_k; ++j) dp[j] = 1;
    for (idx_t s = 1; s <= max_n; ++s) dp[s * (max_k + 1) + 0] = 0;

    /* Compute partition numbers.  Standard DP:
         p(s, j) = p(s, j-1) + p(s-j, j)  if s >= j
    */
    for (idx_t j = 1; j <= max_k; ++j) {
        for (idx_t s = 1; s <= max_n; ++s) {
            seq_t val = dp[s * (max_k + 1) + (j - 1)];
            if (s >= j) val += dp[(s - j) * (max_k + 1) + j];
            dp[s * (max_k + 1) + j] = val;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;        /* a(0) = 1 by definition */

    /* Ensure the partition table covers this n. */
    if (dp == NULL || n > dp_n) build_dp(n);

    seq_t res = 0;
    for (idx_t k = 1;; ++k) {
        idx_t m   = k >> 1;              /* floor(k/2) */
        idx_t cost = k * m;              /* weight contributed by the k-th part */
        if (cost > n) break;             /* no further terms possible */
        idx_t rem = n - cost;            /* remaining sum to partition */
        idx_t largest = k - 1;           /* largest allowed part size */
        /* largest <= dp_k is guaranteed because dp_k >= sqrt(2*dp_n) */
        res += dp[rem * (dp_k + 1) + largest];
    }
    return res;
}
