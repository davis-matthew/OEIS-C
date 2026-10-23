/*
 * A170677  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + 42 * sum_{i=0}^{n-1} a(i) for 0 < n < 49, a(0) = 1
 *            a(n) = 42 * sum_{i=1}^{48} a(n-i) - 903 * a(n-49) for n >= 49
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements actually stored (size) */
    static size_t cache_cap = 0;   /* malloced capacity */
    static seq_t cum_sum = 0;      /* sum of cache[0 .. cache_len-1] (valid while cache_len<=49) */

    /* helper to ensure capacity */
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
        while (n + 1 > new_cap)
            new_cap <<= 1;

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* out of memory – unlikely in tests */

        /* copy existing values (safe for all seq_t types) */
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        size_t i = (size_t)k;   /* array index (k fits in size_t here) */

        if (k == 0) {
            cache[i] = (seq_t)1;
            cum_sum = (seq_t)1; /* sum of a[0] */
        } else if (k < 49) {
            /* a(k) = 2 + 42 * sum_{i=0}^{k-1} a(i) */
            cache[i] = (seq_t)2 + (seq_t)42 * cum_sum;
            cum_sum += cache[i];
        } else {
            /* a(k) = 42 * sum_{i=1}^{48} a(k-i) - 903 * a(k-49) */
            seq_t sum48 = (seq_t)0;
            for (int j = 1; j <= 48; ++j)
                sum48 += cache[i - j];
            cache[i] = (seq_t)42 * sum48 - (seq_t)903 * cache[i - 49];
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
