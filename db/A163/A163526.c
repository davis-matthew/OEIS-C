/*
 * A163526  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=26, a(2)=650, a(3)=16250, a(4)=406250. For n=5
 *            a(5)=24*a(4)+24*a(3)+24*a(2)+24*a(1)-300*a(0)+1 For n>5
 *            a(n)=24*a(n-1)+24*a(n-2)+24*a(n-3)+24*a(n-4)-300*a(n-5). This
 *            recurrence follows from the generating function
 *            (t^5+2t^4+2t^3+2t^2+2t+1)/(300t^5-24t^4-24t^3-24t^2-24t+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;      /* capacity of cache in elements */
    static idx_t cache_len  = 0;      /* number of valid entries in cache */

    /* initial five values are known */
    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)26,
        (seq_t)650,
        (seq_t)16250,
        (seq_t)406250
    };

    /* Handle trivial case */
    if (n < 5)
        return init[n];

    /* Initialise cache if it does not exist */
    if (cache == NULL) {
        /* Ensure at least capacity for the requested index plus one slot,
           but at least five entries because we know them explicitly. */
        cache_cap = (size_t)(n + 1);
        if (cache_cap < 5)
            cache_cap = 5;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();      /* out of memory */
        for (int i = 0; i < 5; ++i)
            cache[i] = init[i];
        cache_len = 5;
    }
    /* Grow cache if necessary */
    else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();      /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute and cache any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term =
            (seq_t)24 * cache[i-1] +
            (seq_t)24 * cache[i-2] +
            (seq_t)24 * cache[i-3] +
            (seq_t)24 * cache[i-4] -
            (seq_t)300 * cache[i-5];
        if (i == 5)                 /* special inhomogeneous part */
            term += (seq_t)1;
        cache[i] = term;
    }
    cache_len = n + 1;   /* cache now contains terms up to n */

    return cache[n];
}
