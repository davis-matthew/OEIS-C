/*
 * A175798  Expansion of ( -3+x+4*x^2-3*x^3+3*x^5-x^7-3*x^4+x^6 ) / ( (1+x)
 *          *(x^5-x^4-x^3+x^2-1) *(x-1)^2 ).
 * https://oeis.org/A175798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = +a(n-1) +2*a(n-2) -3*a(n-3) -a(n-4) +4*a(n-5) -a(n-6)
 *            -2*a(n-7) +a(n-8), with initial values a(0)=3, a(1)=2, a(2)=4,
 *            a(3)=2, a(4)=4, a(5)=3, a(6)=5, a(7)=6
 * Range:     i8 n <= 44, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* first index not yet computed */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* Initial values for n = 0..7 */
    const seq_t init[8] = {3, 2, 4, 2, 4, 3, 5, 6};

    /* If the value is already computed, return it immediately */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Ensure the cache has enough capacity to hold index n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 8;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* Allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i < 8)
            cache[i] = init[i];
        else
            cache[i] = cache[i - 1] + 2 * cache[i - 2] - 3 * cache[i - 3]
                         - cache[i - 4] + 4 * cache[i - 5]
                         - cache[i - 6] - 2 * cache[i - 7] + cache[i - 8];
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
