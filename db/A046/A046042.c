/*
 * A046042  Number of partitions of n into fourth powers.
 * https://oeis.org/A046042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into fourth powers (OEIS
 *            A046042). Computed by a classic coin‑change DP where coins are p
 *            = j^4 (j ≥ 1). The table is built incrementally and recomputed
 *            in full each time a new maximum n is requested. This guarantees
 *            correctness for arbitrary precision seq_t
 * Range:     u8 n <= 626, u16 n <= 3544, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *dp          = NULL;   /* dp[i] = a(i) */
static idx_t  dp_size     = 0;      /* number of elements, i.e. dp_size = max_n + 1 */

/* Recompute the DP table up to new_n (inclusive). */
static void extend(idx_t new_n)
{
    /* Allocate space for dp[0..new_n] */
    seq_t *new_dp = (seq_t *)realloc(dp, (new_n + 1) * sizeof(seq_t));
    if (!new_dp) abort();          /* allocation failed */
    dp = new_dp;
    dp_size = new_n + 1;

    /* Initialise */
    dp[0] = (seq_t)1;              /* one way to partition 0 */
    for (idx_t i = 1; i <= new_n; ++i) dp[i] = (seq_t)0;

    /* Classic unbounded coin change DP: for each fourth power p
       update all sums i >= p. */
    for (idx_t j = 1;; ++j) {
        idx_t p = j * j * j * j;   /* j^4, fits in idx_t for j up to current root */
        if (p > new_n) break;
        for (idx_t i = p; i <= new_n; ++i) {
            dp[i] += dp[i - p];
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* a(0) is undefined, but return 0 for safety */

    if (dp == NULL || n >= dp_size - 1) {
        extend(n);
    }
    return dp[n];
}
