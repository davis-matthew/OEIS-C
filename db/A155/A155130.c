/*
 * A155130  a(n) = 7*a(n-1) + 7*a(n-2), n>2, a(0)=1, a(1)=6, a(2)=48.
 * https://oeis.org/A155130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 7*a(n-2) for n>2, a(0)=1, a(1)=6, a(2)=48
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th term of OEIS sequence A155130. */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0; /* number of computed terms */

    /* Initialize cache with the first three terms */
    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t*)malloc((size_t)cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)48;
    }

    if (n < cache_len) {
        return cache[n];
    }

    /* Ensure the cache is large enough */
    idx_t new_len = n + 1;
    if (new_len > cache_len) {
        cache = (seq_t*)realloc(cache, (size_t)new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)7 * cache[i-1] + (seq_t)7 * cache[i-2];
        }
        cache_len = new_len;
    } else { /* build up to new_len if not already stored */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)7 * cache[i-1] + (seq_t)7 * cache[i-2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
