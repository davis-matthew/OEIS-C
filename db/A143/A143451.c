/*
 * A143451  Expansion of 1/(x^k*(1-x-2*x^(k+1))) for k=8.
 * https://oeis.org/A143451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n+1 for 0 ≤ n ≤ 9; a(n) = a(n-1) + 2*a(n-9) for n > 9
 * Range:     u8 n <= 18, u16 n <= 41, u32 n <= 86, u64 n <= 176,
 *            u128 n <= 356, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t  cached_max = 0;   /* highest index already computed */
    static size_t cap = 0;          /* allocated array size */

    /* Initialise cache with first 10 terms if not yet done */
    if (cache == NULL) {
        cap = 10;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();  /* allocation failed */

        for (idx_t i = 0; i < 10; ++i)
            cache[i] = (seq_t)( (seq_t)2 * (seq_t)i + 1 );

        cached_max = 9;
    }

    /* Extend cache when a larger index is requested */
    if (n > cached_max) {
        /* grow capacity to at least n+1 */
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;
        cap = new_cap;

        /* compute terms up to n */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i <= 9)
                cache[i] = (seq_t)( (seq_t)2 * (seq_t)i + 1 );
            else
                cache[i] = cache[i-1] + (seq_t)2 * cache[i-9];
        }
        cached_max = n;
    }

    return cache[n];
}
