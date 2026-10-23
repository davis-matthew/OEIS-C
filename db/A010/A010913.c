/*
 * A010913  Pisot sequence E(3,17), a(n) = floor( a(n-1)^2/a(n-2)+1/2 ).
 * https://oeis.org/A010913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=3, a(1)=17, a(n)=6*a(n-1)-2*a(n-2) for n>=2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>   /* for memcpy */

/* Persistent cache for memoised values */
static seq_t *cache   = NULL;      /* array of seq_t values */
static size_t cache_cap = 0;       /* allocated size of cache */
static idx_t  next_computed = 0;   /* first index not yet computed */

/* Initialise cache with the first two terms */
static void init_cache(void)
{
    size_t init_cap = 2;
    seq_t *tmp;
#ifdef __cplusplus
    tmp = new seq_t[init_cap];
#else
    tmp = (seq_t*)calloc(init_cap, sizeof(seq_t));
#endif
    tmp[0] = 3;
    tmp[1] = 17;
    cache = tmp;
    cache_cap = init_cap;
    next_computed = 2;
}

/* Ensure the cache can hold indices up to n */
static void grow_cache_to(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;   /* already large enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < cache_cap; ++i)
        tmp[i] = cache[i];
    delete [] cache;
#else
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (!cache)
        init_cache();

    /* Special case for the first two terms */
    if (n == 0)
        return cache[0];
    if (n == 1)
        return cache[1];

    /* Ensure enough room for n */
    grow_cache_to(n);

    /* Compute any missing values up to n */
    for (idx_t i = next_computed; i <= n; ++i) {
        cache[i] = (seq_t)6 * cache[i-1] - (seq_t)2 * cache[i-2];
    }
    next_computed = n + 1;

    return cache[n];
}
