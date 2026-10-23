/*
 * A015566  Expansion of x/(1 - 7*x - 9*x^2).
 * https://oeis.org/A015566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 9*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array of computed values */
static size_t cache_size = 0;     /* allocated size of cache array   */
static idx_t   max_n   = 0;       /* highest index computed so far   */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Initialize cache if first call */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* malloc failure, fallback */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_n = 1;
    }

    /* Return already computed value */
    if (n <= max_n) return cache[n];

    /* Grow cache if necessary */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = cache_size;
        while (new_size <= (size_t)(n + 1))
            new_size <<= 1; /* double until enough */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0; /* realloc failure, fallback */
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing values */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        cache[i] = (seq_t)7 * cache[i - 1] + (seq_t)9 * cache[i - 2];
    }
    max_n = n;
    return cache[n];
}
