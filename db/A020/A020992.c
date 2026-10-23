/*
 * A020992  a(n) = a(n-1) + a(n-2) + a(n-3), with a(0) = 0, a(1) = 2, a(2) =
 *          1.
 * https://oeis.org/A020992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), with a(0)=0, a(1)=2, a(2)=1
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* small static cache to handle repeated calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* handle the defined initial values directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;

    /* initialise cache once */
    if (cache_len == 0) {
        cache_cap = 16;                      /* start with a small capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure: return 0 (unlikely) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache_len = 3;
    }

    /* grow cache if we need more terms */
    if (n >= cache_len) {
        /* ensure we have enough space */
        size_t required = (size_t)n + 1;
        if (required > cache_cap) {
            size_t new_cap = cache_cap;
            while (required > new_cap)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;         /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
