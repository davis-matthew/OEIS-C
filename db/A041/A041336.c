/*
 * A041336  Numerators of continued fraction convergents to sqrt(182).
 * https://oeis.org/A041336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 54*a(n-2) - a(n-4) Initial values: a(0)=13, a(1)=27,
 *            a(2)=715, a(3)=1457
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* keep a computed cache across calls */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* number of entries currently stored */
    static const seq_t init[4] = {13, 27, 715, 1457};

    /* return small indices directly */
    if (n < 4)
        return init[(size_t)n];

    /* initialise cache on first use, or expand it to 4 if it was smaller */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) return 0;      /* OOM – unlikely in the test harness */
        for (size_t i = 0; i < 4; ++i)
            cache[i] = init[i];
    } else if (cache_size < 4) {
        cache_size = 4;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_size);
        for (size_t i = 0; i < 4; ++i)
            cache[i] = init[i];
    }

    /* If value already computed, return it */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Grow cache to hold requested index */
    size_t new_size = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (cache == NULL) return 0;          /* OOM – unlikely */

    /* Compute terms from cached size up to n */
    for (size_t i = cache_size; i <= n; ++i) {
        /* recurrence uses a(i-2) and a(i-4), which are already stored */
        seq_t term = 54 * cache[i-2] - cache[i-4];
        cache[i] = term;
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
