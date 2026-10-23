/*
 * A162816  a(n) = 12*a(n-1)-33*a(n-2) for n > 1; a(0) = 5, a(1) = 33.
 * https://oeis.org/A162816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 33*a(n-2), a(0) = 5, a(1) = 33
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow calls in any order */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of terms currently stored */
    static idx_t cache_cap = 0;    /* allocated capacity */

    if (cache == NULL) {
        /* initial capacity: power of two ≥ n+1 */
        cache_cap = 2;
        while (cache_cap <= n) cache_cap <<= 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();

        /* initialise first two terms */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)33;
        cache_len = 2;
    }

    /* grow capacity if required */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)12 * cache[i - 1] - (seq_t)33 * cache[i - 2];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
