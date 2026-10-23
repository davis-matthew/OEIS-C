/*
 * A138525  a(n) = Sum_{k=0..n} (2*k)!.
 * https://oeis.org/A138525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (2*k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  Static cache of a(i) values and current factorial */
static seq_t *cache = NULL;      /* array of a(i) */
static size_t cache_cap = 0;     /* current capacity of cache */
static idx_t  cache_max = (idx_t)-1;  /* highest n for which a(n) is cached */
static seq_t  fact = 0;          /* (2*cache_max)! for cached cache_max */

/* Ensure cache has room for at least N+1 elements (indices 0..N) */
static void ensure_cache_capacity(idx_t N)
{
    if ((size_t)(N + 1) <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)(N + 1))
        new_cap <<= 1;                 /* double until enough */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure: return 0 */
        cache[0] = (seq_t)1;          /* a(0) = 0! = 1 */
        cache_max = 0;
        fact = (seq_t)1;              /* (2*0)! = 1 */
    }

    if (n <= cache_max)
        return cache[n];

    /* Extend cache up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        idx_t odd  = 2 * k - 1;
        idx_t even = 2 * k;
        fact *= (seq_t)odd;   /* (2k-1)! part */
        fact *= (seq_t)even;  /*  (2k)!  part */
        seq_t new_sum = cache[cache_max] + fact;
        ensure_cache_capacity(k);
        cache[k] = new_sum;
        cache_max = k;
    }
    return cache[n];
}
