/*
 * A171067  Expansion of g.f. x*(1 - x)*(1 + x)/((x^2 + 3*x + 1)*(x^2 - 4*x +
 *          1)).
 * https://oeis.org/A171067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 10*a(n-2) + a(n-3) - a(n-4); Initial values:
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=10;
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache = (seq_t *)0;   /* pointer to array of terms */
    static size_t cache_size = 0;       /* current allocated size (terms) */

    /* initialize cache with first four terms if not done yet */
    if (cache == (seq_t *)0) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();  /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)10;
    }

    /* if the term is already in the cache, return it */
    if ((size_t)n < cache_size) {
        return cache[n];
    }

    /* enlarge cache to hold term n */
    size_t new_size = (size_t)n + 1;
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute the missing terms using the recurrence */
    for (size_t i = 4; i <= (size_t)n; ++i) {
        /* i >= 4 guaranteed because cache already has first four terms */
        cache[i] = cache[i-1] + (seq_t)10 * cache[i-2] + cache[i-3] - cache[i-4];
    }

    return cache[n];
}
