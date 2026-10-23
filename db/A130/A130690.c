/*
 * A130690  Number of partitions of n such that the largest part is coprime to
 *          every other part.
 * https://oeis.org/A130690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of n whose largest part is coprime
 *            to every other part. The implementation uses dynamic
 *            programming: For every possible largest part m (m ≥ 2) we count
 *            partitions of n-m into parts that are < m and coprime with m.
 *            The total for n is the sum of these contributions plus...
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Cache of already computed values */
    static seq_t *cache = NULL;      /* array of a[0..cached_n] */
    static size_t cached_n = 0;      /* maximum n for which cache is valid */

    size_t req = (size_t)n;          /* size_t is large enough for indices */

    /* Recompute cache if requested n is larger than cached_n */
    if (req >= cached_n) {
        size_t new_len = req + 1;    /* length of array needed */

        /* Reallocate cache to hold values up to req */
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {            /* allocation failed */
            /* for safety return 0 on failure */
            return 0;
        }
        cache = new_cache;

        /* Initialize all entries to 0 */
        for (size_t i = cached_n; i < new_len; ++i)
            cache[i] = 0;

        /* Base case: a(0) = 1 */
        cache[0] = 1;
        /* m = 1 contributes exactly one partition (all ones) for n ≥ 1 */
        for (size_t i = 1; i < new_len; ++i)
            cache[i] = 1;

        /* Contributions from largest part m ≥ 2 */
        for (size_t m = 2; m < new_len; ++m) {
            size_t limit = new_len - m - 1;          /* maximum remaining sum */
            /* Allocate DP array for partitions of sum up to 'limit' */
            seq_t *dp = (seq_t *)calloc(limit + 1, sizeof(seq_t));
            if (!dp) {                               /* allocation failed */
                free(cache);
                cache = NULL;
                cached_n = 0;
                return 0;
            }
            dp[0] = 1;    /* empty partition */

            /* For each part size d < m that is coprime to m,
               update the DP array (unbounded coin change) */
            for (size_t d = 1; d < m; ++d) {
                if (gcd((seq_t)d, (seq_t)m) != 1)
                    continue;
                for (size_t i = d; i <= limit; ++i)
                    dp[i] += dp[i - d];
            }

            /* Add the partitions of n-m to cache entries */
            for (size_t k = 0; k <= limit; ++k) {
                cache[m + k] += dp[k];
            }

            free(dp);
        }

        cached_n = new_len;   /* cache now valid up to 'req' */
    }

    return cache[req];
}
