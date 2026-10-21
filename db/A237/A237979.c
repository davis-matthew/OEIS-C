/*
 * A237979  Number of strict partitions of n such that (least part) > number
 *          of parts.
 * https://oeis.org/A237979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{⌊√n⌋} p_distinct(n – k², k) where p_distinct(m,
 *            k) is the number of partitions of m into exactly k distinct
 *            positive integers. Recurrence: p_distinct(m, k) = p_distinct(m –
 *            k, k) + p_distinct(m – k, k – 1) (for m ≥ k). The table is
 *            memoised up to the largest n requested
 * Range:     u8 n <= 46, u16 n <= 120, u32 n <= 357, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Cached DP table */
static idx_t dp_N = 0;       /* largest n currently computed */
static size_t dp_K = 0;      /* largest k currently used (≈ √dp_N) */
static seq_t *dp = NULL;     /* flat array: dp[i][k] → dp[i*cols + k] */

/* Build or extend the DP table to cover all n up to nmax */
static void ensure_dp(idx_t nmax)
{
    if (dp_N >= nmax) return;            /* already sufficient */

    if (dp) { free(dp); dp = NULL; }     /* discard old table */

    dp_N = nmax;
    dp_K = (size_t)sqrt((double)nmax);    /* floor(√nmax) */

    size_t rows = dp_N + 1;
    size_t cols = dp_K + 1;
    dp = (seq_t *)calloc(rows * cols, sizeof(seq_t));
    if (dp == NULL) return;              /* allocation failed – caller will get zeros */

    dp[0] = 1;                            /* dp[0][0] = 1 */

    /* DP recurrence */
    for (idx_t i = 1; i <= dp_N; ++i) {
        size_t i_base = i * cols;
        for (size_t k = 1; k <= dp_K; ++k) {
            if (i >= k) {
                idx_t src = i - k;
                size_t src_base = src * cols;
                dp[i_base + k] = dp[src_base + k] + dp[src_base + (k - 1)];
            }
            /* else: dp[i][k] stays 0 (calloc initialised) */
        }
    }
}

/* Required OEIS function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for offset < 1 */

    ensure_dp(n);                        /* guarantee table exists */

    size_t cols = dp_K + 1;
    size_t sqrt_n = (size_t)sqrt((double)n);
    seq_t result = 0;

    for (size_t k = 1; k <= sqrt_n; ++k) {
        idx_t m = n - (idx_t)(k * k);    /* m = n – k² ≥ 0 by loop order */
        result += dp[m * cols + k];
    }

    return result;
}
