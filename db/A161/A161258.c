/*
 * A161258  Number of partitions of n into central polygonal numbers A000124
 *          where every part appears at least 5 times.
 * https://oeis.org/A161258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 235, u32 n <= 939, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A161258
   Formula: a(n) = number of partitions of n into central polygonal
   numbers A000124 (1,2,4,7,11,16, …) such that every part used
   appears at least 5 times.
*/
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Allocate DP arrays (size n+1).  seq_t is an integer type. */
    size_t size = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *h  = (seq_t *)calloc(size, sizeof(seq_t));
    if (!dp || !h) {
        /* In a real build we would handle allocation failure.
           Here we simply return 0. */
        if (dp) free(dp);
        if (h)  free(h);
        return 0;
    }

    /* Base case: one way to form sum 0 (empty partition). */
    dp[0] = 1;

    /* Process each central polygonal number v = i*(i-1)/2 + 1. */
    for (idx_t i = 1;; ++i) {
        /* Compute v; stop when v > n. */
        idx_t v = (i * (i - 1)) / 2 + 1;
        if (v > n) break;

        /* Copy current dp to h (old DP). */
        for (idx_t j = 0; j <= n; ++j) h[j] = dp[j];

        /* h now represents partitions using unlimited copies of v
           on top of the previous coin restrictions. */
        for (idx_t j = v; j <= n; ++j)
            h[j] += h[j - v];

        /* Add contributions where at least 5 copies of v are used. */
        if (v <= n / 5) {
            idx_t start = v * 5;
            for (idx_t j = start; j <= n; ++j)
                dp[j] += h[j - start];
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(h);
    return result;
}
