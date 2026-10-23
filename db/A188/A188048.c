/*
 * A188048  Expansion of (1 - x^2)/(1 - 3*x^2 - x^3).
 * https://oeis.org/A188048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + a(n-3), a(0)=1, a(1)=0, a(2)=2 Generating
 *            function: (1 - x^2)/(1 - 3*x^2 - x^3)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 72, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last   = 0;            /* highest index already computed */

    /* initialise cache with the base cases */
    if (cache == NULL) {
        cap   = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();    /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        last    = 2;
    }

    /* enlarge the cache if n is beyond its current size */
    if ((size_t)n >= cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();      /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* compute all missing values up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* i >= 3 here because last >= 2 initially */
            cache[i] = (seq_t)(3 * cache[i - 2] + cache[i - 3]);
        }
        last = n;
    }

    return cache[(size_t)n];
}
