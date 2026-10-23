/*
 * A106539  a(1)=1, a(2)=1, a(n) = (n-1)*a(n-1) - (n-2)*a(n-2) - ... - a(1)
 *          for n>=3.
 * https://oeis.org/A106539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n*a(n-1) - 2*(n-2)*a(n-2) with a(1)=1, a(2)=1
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[1] .. cache[cache_cap-1] */
static size_t cache_cap = 0;  /* number of allocated entries (>=3 after init) */
static idx_t  cache_last = 0; /* highest computed index */

seq_t a(idx_t n) {
    /* initialize the cache on first call */
    if (cache == NULL) {
        /* reserve indices 0..2; we will use 1 and 2 */
        cache_cap = 3;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_last = 2;
    }

    /* if n is within cached range, return immediately */
    if (n <= cache_last) return cache[n];

    /* grow cache to hold index n */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t exp1 = (seq_t)i * cache[i - 1];
        seq_t exp2 = (seq_t)2 * (seq_t)(i - 2) * cache[i - 2];
        cache[i] = exp1 - exp2;
    }
    cache_last = n;
    return cache[n];
}
