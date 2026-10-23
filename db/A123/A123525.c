/*
 * A123525  Arises in the normal ordering of functions of a*(a+)*a, where a
 *          and a+ are the boson annihilation and creation operators,
 *          respectively.
 * https://oeis.org/A123525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence * (n-2)*(n-1)*a(n) = 2*(n-2)*n^2*a(n-1) -
 *            (n-1)^3*n*a(n-2) * Initial values: a(1) = 2, a(2) = 14
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* 1‑based memoisation cache */
static seq_t *cache = NULL;   /* cache[0] unused */
static size_t cache_cap = 0;  /* allocated capacity (>= 1 if used) */
static idx_t   cache_max = 0; /* highest n computed */

/* Compute the next term (n >= 3) using the recurrence. */
#ifdef __cplusplus
/* C++ build – seq_t may be an arbitrary‑precision class. */
static seq_t compute_next(idx_t n, seq_t prev, seq_t prev2)
{
    /* All operations are performed with seq_t, which is large enough
       in the C++ build. Overflow cannot occur because the class
       supports arbitrary precision. */
    return ( ( (seq_t)2 * (n-2) * n * n * prev )
           - ( (n-1) * (n-1) * (n-1) * n * prev2 )
         ) / ( (n-2) * (n-1) );
}
#else
/* C build – use unsigned __int128 for intermediate to avoid overflow. */
static seq_t compute_next(idx_t n, seq_t prev, seq_t prev2)
{
    unsigned __int128 num1 =
        (unsigned __int128)2 * (n-2) * n * n * prev;
    unsigned __int128 num2 =
        (unsigned __int128)(n-1) * (n-1) * (n-1) * n * prev2;
    unsigned __int128 denom =
        (unsigned __int128)(n-2) * (n-1);
    return (seq_t)((num1 - num2) / denom);
}
#endif

/* Ensure the cache can hold indices up to 'needed'. */
static void ensure_cache(idx_t needed)
{
    if (cache_cap > needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while ((idx_t)new_cap <= needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)14;

    /* initialise the cache on first use */
    if (!cache) {
        /* allocate space for indices 0..2 (0 unused) */
        cache = (seq_t*)calloc(3, sizeof(seq_t));
        if (!cache)
            abort();
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)14;
        cache_cap = 3;
        cache_max = 2;
    }

    if (n <= cache_max)
        return cache[n];

    /* extend cache up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        ensure_cache(i);
        cache[i] = compute_next(i, cache[i-1], cache[i-2]);
    }
    cache_max = n;
    return cache[n];
}
