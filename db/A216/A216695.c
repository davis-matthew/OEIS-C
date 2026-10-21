/*
 * A216695  Number of compositions (ordered partitions) of n into at least 2
 *          distinct positive parts.
 * https://oeis.org/A216695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A032020(n)-1, where A032020(n) = Σ_{k≥1} k! * p(n,k) and
 *            p(n,k) is the number of partitions of n into k distinct parts
 * Range:     u8 n <= 14, u16 n <= 31, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   max_k_for_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/max_k_for_n.h"

/* main sequence routine */
seq_t a(idx_t n) {
    /* quick answer for the offset */
    if (n == 0) return 0;

    /* static state */
    static seq_t *dp = NULL;
    static seq_t *fac = NULL;
    static idx_t current_n = 0;
    static size_t current_k = 0;

    /* if we have not computed enough, recompute up to n */
    if (n > current_n) {
        /* new limits */
        idx_t new_n = n;
        size_t new_k = (size_t)max_k_for_n(new_n);

        /* allocate new storage */
        size_t rows = (size_t)(new_n + 1);
        size_t cols = new_k + 1;                 /* include 0..k */
        seq_t *new_dp = (seq_t *)calloc(rows * cols, sizeof(seq_t));
        if (!new_dp) return 0; /* out of memory, return 0 as fallback */

        /* recurrence for partitions into distinct parts: p[n][k] = p[n-k][k] + p[n-k][k-1] */
        new_dp[0 * cols + 0] = 1;  /* p(0,0) = 1 */
        for (idx_t i = 1; i <= new_n; ++i) {
            for (size_t k = 1; k <= new_k; ++k) {
                if (i < k) continue; /* impossible to partition i into k distinct parts */
                size_t pos   = (size_t)i * cols + k;
                size_t a_idx = (size_t)(i - k) * cols + k;
                size_t b_idx = (size_t)(i - k) * cols + (k - 1);
                new_dp[pos] = new_dp[a_idx] + new_dp[b_idx];
            }
        }

        /* factorials up to new_k */
        seq_t *new_fac = (seq_t *)malloc((new_k + 1) * sizeof(seq_t));
        if (!new_fac) {
            free(new_dp);
            return 0; /* out of memory, return 0 as fallback */
        }
        new_fac[0] = 1;
        for (size_t i = 1; i <= new_k; ++i) {
            new_fac[i] = new_fac[i - 1] * (seq_t)i;
        }

        /* replace old tables */
        if (dp) free(dp);
        if (fac) free(fac);
        dp = new_dp;
        fac = new_fac;
        current_n = new_n;
        current_k = new_k;
    }

    /* compute a(n) = Σ_{k≥1} k! * p(n,k) - 1 */
    seq_t sum = 0;
    size_t cols = current_k + 1;
    for (size_t k = 1; k <= current_k; ++k) {
        seq_t part = dp[(size_t)n * cols + k];
        if (part) sum += fac[k] * part;
    }
    return sum - 1;
}
