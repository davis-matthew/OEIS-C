/*
 * A117576  Expansion of (1-x^3)/((1-x^2)*(1+2*x^2)).
 * https://oeis.org/A117576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1) - 2*a(n-2) - 2*a(n-3) initial values: a(0)=1,
 *            a(1)=0, a(2)=-1
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation buffer          */
static idx_t cache_size = 0;     /* allocated size (in seq_t)   */
static idx_t last_computed = 0;  /* highest index computed so far */

seq_t a(idx_t n)
{
    /* Initialise the cache with the first three terms */
    if (cache == NULL) {
        size_t init_size = 3;
        if ((size_t)(n + 1) > init_size)
            init_size = (size_t)(n + 1);
        cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = -(seq_t)1;
        cache_size = (idx_t)init_size;
        last_computed = 2;
    }

    /* Extend the cache if the requested index exceeds the current size */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_size = (idx_t)new_size;
    }

    /* Compute any missing terms up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = -cache[i-1]
                       - (seq_t)2 * cache[i-2]
                       - (seq_t)2 * cache[i-3];
        }
        last_computed = n;
    }

    return cache[n];
}
