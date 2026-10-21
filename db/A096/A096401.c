/*
 * A096401  Number of balanced partitions of n into distinct parts: least part
 *          is equal to number of parts.
 * https://oeis.org/A096401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k>=1, k^2 <= n} p_distinct_exact(n - k^2, k-1) where
 *            p_distinct_exact(m,t) is the number of partitions of m into
 *            exactly t distinct positive parts. This is computed via the
 *            transformation p_distinct_exact(m,t) = p_unrestricted_exact(m -
 *            t(t-1)/2, t) (m - t(t-1)/2 >= 0) with p_unrestri...
 * Range:     u8 n <= 55, u16 n <= 133, u32 n <= 377, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static seq_t *dp_un = NULL;          /* flat 2‑D table for unrestricted partitions */
static size_t dp_rows = 0;           /* number of rows (max n + 1) */
static size_t dp_cols = 0;           /* number of columns (max t + 1) */

/* Ensure that dp_un contains values for all s <= n and t <= max_t(n). */
static void ensure_dp_un(idx_t n)
{
    if ((size_t)n < dp_rows) return;          /* already large enough */

    /* compute new table size */
    size_t new_rows = (size_t)n + 1;
    size_t new_cols = (size_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0) + 1; /* max t + 1 */

    /* allocate new table */
    seq_t *new_dp = (seq_t *)calloc(new_rows * new_cols, sizeof(seq_t));
    if (!new_dp) abort();                 /* out of memory */

    /* base case */
    new_dp[0 * new_cols + 0] = 1;

    /* DP: p_unrestricted_exact(s,t) */
    for (size_t s = 1; s < new_rows; ++s) {
        for (size_t t = 1; t < new_cols; ++t) {
            seq_t val = new_dp[(s-1) * new_cols + (t-1)]; /* p(s-1, t-1) */
            if (s >= t) {
                val += new_dp[(s - t) * new_cols + t];     /* p(s-t, t) */
            }
            new_dp[s * new_cols + t] = val;
        }
    }

    /* free old table */
    free(dp_un);
    dp_un = new_dp;
    dp_rows = new_rows;
    dp_cols = new_cols;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not defined for offset 1, but harmless */

    ensure_dp_un(n);

    seq_t res = 0;

    /* iterate over k = number of parts (and minimal part) */
    for (idx_t k = 1; (idx_t)((int)k * k) <= n; ++k) {
        idx_t m = n - (idx_t)((int)k * k);   /* remainder after fixing k */
        idx_t t = k - 1;                     /* remaining parts */

        if (t == 0) {
            /* need m == 0 for a valid partition of 0 into 0 parts */
            if (m == 0) res += 1;
            continue;
        }

        idx_t sub = (idx_t)t * (t - 1) / 2;  /* subtraction for transformation */
        if (m < sub) continue;

        idx_t s_prime = m - sub;
        if (s_prime + 1 > dp_rows || t + 1 > dp_cols) continue;  /* safety */
        res += dp_un[s_prime * dp_cols + t];
    }

    return res;
}
