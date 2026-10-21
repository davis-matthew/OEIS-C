/*
 * A239467  Number of 1-separable partitions of n; see Comments.
 * https://oeis.org/A239467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into at least two parts such
 *            that * the number of 1‑parts differs from the number of parts
 *            greater than 1 by at most one
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;          /* cache of computed a(n) values          */
static size_t cache_size = 0;    /* size of the cache (maximum n computed) */

static void precompute(idx_t up)
{
    size_t N = (size_t)up;
    if (A != NULL) {
        free(A);
    }
    A = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (A == NULL) exit(1);

    /* dp[m] will hold p(k, m) – the number of partitions of m into parts
     * not exceeding the current k.  We update dp incrementally for k = 1 .. max_k.
     */
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (dp == NULL) exit(1);
    dp[0] = 1;                   /* empty partition */

    idx_t max_k = (idx_t)(N / 3 + 1);   /* k required for odd‑B terms */
    for (idx_t k = 1; k <= max_k; ++k) {
        /* Update dp to count partitions using parts up to k. */
        for (idx_t m = k; m <= N; ++m) {
            dp[m] += dp[m - k];
        }

        /* ---------- Even case :  r = s = k ----------
         * Contributes to a[n] where n >= 3k, using p(k, n-3k).
         */
        if (k <= N / 3) {
            for (idx_t n = 3 * k; n <= N; ++n) {
                A[n] += dp[n - 3 * k];
            }
        }

        /* ---------- Odd case A :  r = k+1 , s = k ----------
         * Contributes to a[n] where n >= 3k+1, using p(k, n-3k-1).
         */
        for (idx_t n = 3 * k + 1; n <= N; ++n) {
            A[n] += dp[n - 3 * k - 1];
        }

        /* ---------- Odd case B :  r = k-1 , s = k ----------
         * Contributes to a[n] where n >= 3(k-1)+2, using p(k, n-3(k-1)-2).
         */
        if (k >= 2) {
            for (idx_t n = 3 * (k - 1) + 2; n <= N; ++n) {
                A[n] += dp[n - 3 * (k - 1) - 2];
            }
        }
    }

    free(dp);
    cache_size = N;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    /* Ensure the cache exists and covers the requested index. */
    if (A == NULL || (idx_t)cache_size < n) {
        idx_t new_size = (n < 10000 ? 10000 : n);
        precompute(new_size);
    }
    return A[(size_t)n];
}
