/*
 * A179619  a(1)=1, a(n+1) = 10*a(n)+2*n-1.
 * https://oeis.org/A179619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=10*a(n-1)+2*n-1 for n>=2 */
#include <stdlib.h>

/* external types seq_t and idx_t are provided by the build system */

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;   /* largest n computed so far */

    /* initialise cache for term a(1) if needed */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                /* allocation failure */
        cache[0] = (seq_t)1;                /* a(1) = 1 */
        cache_max = 1;
    }

    /* expand cache if the requested n exceeds current capacity */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];          /* a(i-1) */
        seq_t term = prev * (seq_t)10 + (seq_t)2 * (seq_t)i - (seq_t)1;
        cache[i - 1] = term;                /* store a(i) */
    }
    if (n > cache_max) cache_max = n;

    return cache[n - 1];  /* a(n) is at index n-1 */
}
