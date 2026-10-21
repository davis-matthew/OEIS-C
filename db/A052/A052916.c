/*
 * A052916  Expansion of (1-x)/(1 - x - 2*x^3 + x^4).
 * https://oeis.org/A052916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) – a(n-4) for n ≥ 4 * Initial terms:
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=2 * Recurrence uses only integer
 *            arithmetic, fits the O(n) requirement
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 53, u64 n <= 103,
 *            u128 n <= 203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* cached terms up to cache_size-1 */
static size_t  cache_cap = 0;   /* allocated capacity (in elements) */
static idx_t   cache_size = 0;  /* number of terms already computed */

/* compute a(n) with static memoization */
seq_t a(idx_t n)
{
    /* initialise the cache on first call */
    if (cache_size == 0) {
        cache_cap   = 16;                         /* initial capacity */
        cache       = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;              /* allocation failure */

        /* set the first four terms exactly as required */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)2;
        cache_size = 4;
    }

    /* if requested term already cached, return immediately */
    if (n < cache_size) return cache[n];

    /* grow the cache until it can hold n */
    while (cache_cap <= (size_t)n) cache_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0; /* allocation failure – return 0 as safe fallback */
    cache = tmp;

    /* compute new terms up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* i >= 4 because cache_size starts at 4 */
        cache[i] = cache[i-1] + (cache[i-3] + cache[i-3]) - cache[i-4];
    }
    cache_size = n + 1;

    return cache[n];
}
