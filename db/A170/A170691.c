/*
 * A170691  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = 8 * Σ_{k=1}^{49} a(n-k) – 36 *
 *            a(n-50) + r_n where r_0 = 1, r_n = 2 for 1 ≤ n ≤ 49, r_50 = 1,
 *            r_n = 0 for n > 50. This follows from the rational generating
 *            function (t^50 + 2t^49 + … + 2t + 1) / (36t^50 – 8t^49 – … – 8t
 *            + 1) given in OEIS A170691
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation across calls */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size */
    static size_t cache_len  = 0;   /* number of values already stored (max index + 1) */

    /* Resize cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;                /* out of memory */
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        memset(new_cache + cache_len, 0, (new_size - cache_len) * sizeof(seq_t));
        cache      = new_cache;
        cache_size = new_size;
    }

    /* Initialise base case if cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        cache_len = 1;
    }

    /* Compute values up to index n if needed */
    if (cache_len <= (size_t)n) {
        /* Compute initial window sum for the first new index:
           sum of a_{i-49} … a_{i-1} for i = cache_len.  */
        seq_t win_sum = 0;
        idx_t start = (cache_len > 49) ? cache_len - 49 : 0;
        for (idx_t j = start; j < cache_len; ++j)
            win_sum += cache[j];

        /* Iterate over all required new indices */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* r_i depends only on i */
            seq_t r = (seq_t)0;
            if (i <= 49) {
                /* r_0 = 1 (but i≥1 here), r_{1..49} = 2 */
                if (i == 0)
                    r = (seq_t)1;
                else
                    r = (seq_t)2;
            } else if (i == 50) {
                r = (seq_t)1;
            }

            /* a_{i-50} used only when i ≥ 50; otherwise 0 */
            seq_t sub = (i >= 50) ? cache[i - 50] : (seq_t)0;

            /* Compute a_i from the recurrence */
            cache[i] = (seq_t)8 * win_sum - (seq_t)36 * sub + r;

            /* Update the window sum for the next index (i+1) */
            win_sum += cache[i];
            if (i >= 49) {
                win_sum -= cache[i - 49];
            }
        }

        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
