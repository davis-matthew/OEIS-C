/*
 * A169022  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) =
 *            7*(a(n-1)+...+a(n-23)) - 28*a(n-24) for n >= 25 a(0) = 1 For 1
 *            <= n <= 23: a(n) = 9*8^(n-1) a(24) = 7*(a(23)+...+a(1)) -
 *            28*a(0) + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   free_cache, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* static cache: computed terms 0 .. cache_len-1 */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored      */
    static size_t cache_cap = 0;   /* allocated capacity                    */

    /* Ensure enough capacity for index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 32) new_cap = 32;          /* small minimum heap */
        seq_t *new_cache = seq_alloc_array(new_cap);
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free_cache(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return already computed value */
    if (n < cache_len)
        return cache[n];

    /* Compute missing terms up to n */
    size_t i = cache_len; /* next index to compute */

    /* 0th term */
    if (i == 0) {
        cache[0] = (seq_t)1;
        i = 1;
    }

    /* 1 .. 23 : a(n) = 9*8^(n-1) */
    if (i <= 23 && n >= 1) {
        /* compute first term if missing */
        if (i == 1)
            cache[1] = (seq_t)9;
        for ( ; i <= 23 && i <= n; ++i ) {
            /* i >= 2  */
            if (i > 1)
                cache[i] = cache[i-1] * (seq_t)8;
        }
    }

    /* 24th term special */
    if (i <= 24 && n >= 24) {
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= 23; ++k) {
            sum += cache[24 - k] * (seq_t)7;
        }
        sum -= cache[0] * (seq_t)28;
        sum += (seq_t)1;
        cache[24] = sum;
        i = 25;
    }

    /* Recurrence for n >= 25 */
    for ( ; i <= n; ++i ) {
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= 23; ++k)
            sum += cache[i - k] * (seq_t)7;
        sum -= cache[i - 24] * (seq_t)28;
        cache[i] = sum;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
