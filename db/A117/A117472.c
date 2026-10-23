/*
 * A117472  Values of c in a^2 + b^2 = c^2, where b - a = 17 and gcd(a,b,c)=1.
 * https://oeis.org/A117472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=25, a(2)=53, a(3)=137, a(4)=305,
   a(n)=6*a(n-2)-a(n-4)   for n>4
   (OEIS A117472) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow calls in any order */
    static seq_t *cache = NULL;          /* array of computed terms (index 0 unused) */
    static idx_t cache_allocated = 0;    /* allocated size (>= n+1) */
    static idx_t cache_max = 0;          /* highest index for which cache[i] is valid */

    /* Initial values (indices 1..4) */
    static const seq_t init[5] = {0, 25, 53, 137, 305};

    /* Return directly if we are in the initial segment */
    if (n <= 4) {
        return init[n];
    }

    /* Grow or allocate the cache to accommodate index n */
    if (cache_allocated < n + 1) {
        size_t new_size = (size_t)(n + 1);
        /* Cast required for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        cache = tmp;
        cache_allocated = new_size;
    }

    /* Ensure the first four terms are present in the cache */
    if (cache_max < 4) {
        for (idx_t i = 1; i <= 4; ++i) {
            cache[i] = init[i];
        }
        cache_max = 4;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = 6 * cache[i - 2] - cache[i - 4];
    }
    cache_max = n;

    return cache[n];
}
