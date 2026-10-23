/*
 * A185976  Number of multiset repetition class defining partitions of N with
 *          1<=N<=n.
 * https://oeis.org/A185976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} p_tri(k) where p_tri(k) counts partitions of k
 *            into triangular numbers 1, 3, 6, 10, … . The algorithm builds
 *            p_tri(k) with a classic partition DP and keeps the cumulative
 *            sums in a static array. Complexity: O(n·T) where T is the number
 *            of triangular numbers ≤ n (≈√(2n)), which i...
 * Range:     u8 n <= 21, u16 n <= 85, u32 n <= 399, u64 n <= 2211,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for cumulative sums a[0..max_n]. */
    static seq_t *cum = NULL;
    static idx_t   max_n_cached = 0;

    /* Recompute to a larger n if needed (reuse existing data). */
    if (cum == NULL || n > max_n_cached) {
        /* Determine new size. */
        size_t new_size = (size_t)n + 1;     /* include index 0 */

        /* Allocate new dp and cumulative arrays. */
        seq_t *dp = (seq_t*)calloc(new_size, sizeof(seq_t));
        seq_t *new_cum = (seq_t*)calloc(new_size, sizeof(seq_t));

        if (!dp || !new_cum) {              /* allocation failure: keep old */
            free(dp); free(new_cum);
            return (seq_t)0;
        }

        /* Initial value: one way to partition 0. */
        dp[0] = (seq_t)1;

        /* Generate all triangular numbers up to n. */
        for (idx_t t = 1; t * (t + 1) / 2 <= n; ++t) {
            idx_t tri = t * (t + 1) / 2;
            for (idx_t s = tri; s <= n; ++s) {
                dp[s] += dp[s - tri];
            }
        }

        /* Build cumulative sums for a(n). */
        new_cum[0] = (seq_t)0;
        for (idx_t i = 1; i <= n; ++i) {
            new_cum[i] = new_cum[i - 1] + dp[i];
        }

        /* Replace old cache. */
        free(cum);
        cum = new_cum;
        max_n_cached = n;

        /* Free temporary dp array. */
        free(dp);
    }

    return cum[(size_t)n];
}
