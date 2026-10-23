/*
 * A041607  Denominators of continued fraction convergents to sqrt(321).
 * https://oeis.org/A041607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 430*a(n-4) - a(n-8) for n>7 with initial values a(0)=1,
 *            a(1)=1, a(2)=11, a(3)=12, a(4)=419, a(5)=431, a(6)=4729,
 *            a(7)=5160
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated capacity */
    static size_t cache_upto = 0;         /* highest index that has been computed (inclusive) */

    /* Initial size for the cache : at least 8 to hold the first eight terms */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) { /* OOM – abort the program */
            abort();
        }

        /* fill known initial terms */
        const seq_t init[8] = {1, 1, 11, 12, 419, 431, 4729, 5160};
        for (size_t i = 0; i < 8; ++i) {
            cache[i] = init[i];
        }

        cache_upto = 7;   /* last computed index */
    }

    /* If requested value is already cached, return it */
    if (n <= cache_upto) {
        return cache[n];
    }

    /* Ensure the cache is large enough to hold up to n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)(n + 1)) {
            new_cap = n + 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute recurrence for missing terms up to n */
    for (size_t i = cache_upto + 1; i <= (size_t)n; ++i) {
        /* a(i) = 430*a(i-4) - a(i-8) */
        seq_t t = cache[i - 4];
        t *= (seq_t)430;
        t -= cache[i - 8];
        cache[i] = t;
    }

    cache_upto = n;
    return cache[n];
}
