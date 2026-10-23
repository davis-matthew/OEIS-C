/*
 * A008483  Number of partitions of n into parts >= 3.
 * https://oeis.org/A008483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) - p(n-1) - p(n-2) + p(n-3)
 * Range:     u8 n <= 28, u16 n <= 64, u32 n <= 170, u64 n <= 506,
 *            u128 n <= 1656, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p(k) is the unrestricted partition function */

/* Partition numbers p(k) are cached in the static array dp.
 * The cache is grown on demand using the pentagonal number theorem.
 */
#include <stdlib.h>

static seq_t *dp = NULL;     /* dp[i] holds p(i) */
static idx_t dp_size = 0;    /* number of elements allocated in dp */

/* Ensure that dp[0..n] are computed. */
static void ensure_dp(idx_t n)
{
    /* Initialisation: p(0) = 1 */
    if (dp_size == 0) {
        dp = (seq_t *)calloc(1, sizeof(seq_t));
        if (!dp) abort();
        dp[0] = (seq_t)1;
        dp_size = 1;
    }

    if (dp_size > n) return;

    /* Allocate space up to n */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (!tmp) abort();
    /* initialise new entries to 0 */
    for (size_t i = dp_size; i < new_size; ++i) tmp[i] = (seq_t)0;

    /* Compute partition numbers for i = dp_size .. n */
    for (idx_t i = dp_size; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1;; ++k) {
            /* Generalised pentagonal numbers: k(3k±1)/2 */
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i) break;
            int sign = (k & 1) ? 1 : -1;

            if (sign > 0)
                sum += tmp[i - g1];
            else
                sum -= tmp[i - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > i) continue;
            if (sign > 0)
                sum += tmp[i - g2];
            else
                sum -= tmp[i - g2];
        }
        tmp[i] = sum;
    }

    dp      = tmp;
    dp_size = (idx_t)new_size;
}

/* Sequence A008483: number of partitions of n into parts ≥ 3 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n < 3)  return (seq_t)0;

    ensure_dp(n);

    /* a(n) = p(n) - p(n-1) - p(n-2) + p(n-3) */
    seq_t res = dp[n] + dp[n - 3];
    res -= dp[n - 1];
    res -= dp[n - 2];
    return res;
}
