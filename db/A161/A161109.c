/*
 * A161109  Number of partitions of n into nonzero triangular numbers where
 *          every part appears at least 8 times.
 * https://oeis.org/A161109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into nonzero triangular numbers
 *            where every part appears at least 8 times
 * Range:     u8 n <= 146, u16 n <= 428, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;               /* offset starts at 1 */

    /* static cache for values up to the maximum n seen so far */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* size of cache (n+1) */

    if (cache_size < n + 1) {           /* need to recompute up to n */
        if (cache) {                    /* free old cache if any */
            free(cache);
            cache = NULL;
            cache_size = 0;
        }

        idx_t limit = n;
        size_t size = (size_t)(limit + 1);
        seq_t *dp_old = (seq_t*)calloc(size, sizeof(seq_t));
        seq_t *dp_new = (seq_t*)calloc(size, sizeof(seq_t));
        if (!dp_old || !dp_new) {
            free(dp_old); free(dp_new);
            return 0;
        }
        dp_old[0] = 1;                   /* base case: sum 0 has one partition */

        /* temporary array for prefix sums (size limit+1 reused) */
        seq_t *tmp = (seq_t*)calloc(size, sizeof(seq_t));
        if (!tmp) {
            free(dp_old); free(dp_new);
            return 0;
        }

        /* iterate over triangular numbers t_k = k*(k+1)/2 <= limit */
        for (idx_t k = 1;; ++k) {
            idx_t t = k * (k + 1) / 2;
            if (t > limit) break;

            /* start with old dp values as base for new dp */
            memcpy(dp_new, dp_old, size * sizeof(seq_t));

            /* process each residue modulo t separately */
            for (idx_t r = 0; r < t; ++r) {
                size_t msz = 0;
                /* collect old dp values along this residue */
                for (idx_t idx = r; idx <= limit; idx += t) {
                    tmp[msz] = dp_old[idx];
                    ++msz;
                }

                /* build prefix sums: tmp[i] = sum_{j=0}^{i} old[...] */
                for (size_t i = 1; i < msz; ++i)
                    tmp[i] += tmp[i - 1];

                /* add contributions of at least 8 copies of t */
                for (size_t i = 0; i < msz; ++i) {
                    idx_t idx = r + (idx_t)i * t;
                    if (i >= 8)            /* sum of old values up to i-8 */
                        dp_new[idx] += tmp[i - 8];
                }
            }

            /* swap dp arrays for next triangular number */
            seq_t *swap = dp_old;
            dp_old = dp_new;
            dp_new = swap;
        }

        /* dp_old now contains final counts up to 'limit' */
        cache = dp_old;                   /* keep the final array as cache */
        cache_size = size;
        free(dp_new);                     /* free the unused buffer */
        free(tmp);
    }

    return cache[n];
}
