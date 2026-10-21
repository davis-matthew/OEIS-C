/*
 * A238594  Number of partitions p of n such that 2*min(p) is not a part of p.
 * https://oeis.org/A238594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) – b(n), where b(n) counts partitions of n that *
 *            contain a part equal to twice their minimal part. This
 *            implementation * uses a single DP array that successively counts
 *            partitions with parts * ≥ m. While processing each m, the
 *            algorithm accumulates the required * contribution to B[n]...
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 143, u64 n <= 452,
 *            u128 n <= 1543, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;     /* counts partitions with parts ≥ current m */
static seq_t *B  = NULL;     /* counts partitions where 2*min part is present */
static idx_t max_n = 0;      /* largest n already computed */

/* Recompute tables up to N (inclusive) */
static void recompute(idx_t N)
{
    if (N <= max_n) return;

    /* free previous tables and allocate new ones */
    if (dp) free(dp);
    if (B)  free(B);

    size_t sz = (size_t)(N + 1);
    dp = (seq_t*)calloc(sz, sizeof(seq_t));
    B  = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!dp || !B) exit(1);          /* out‑of‑memory */

    dp[0] = (seq_t)1;                /* empty partition */

    for (idx_t m = N; m >= 1; --m) {
        /* Add partitions that use the part size m */
        for (idx_t j = m; j <= N; ++j) {
            dp[j] += dp[j - m];
        }

        /* Accumulate contributions to B[n] for this minimal part m */
        if (N >= 3 * m) {            /* only if n-3m can be non‑negative */
            idx_t limit = N - 3 * m;
            for (idx_t t = 0; t <= limit; ++t) {
                B[t + 3 * m] += dp[t];
            }
        }
    }

    max_n = N;
}

/* Sequence entry point */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset 1, undefined for n<1 */
    recompute(n);
    return dp[n] - B[n];
}
