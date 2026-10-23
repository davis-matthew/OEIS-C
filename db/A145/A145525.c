/*
 * A145525  Numbers X such that there exists Y in N : X^2=273*Y^2+91.
 * https://oeis.org/A145525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 182, a(2) = 264446, a(n+2) = 1454*a(n+1) – a(n) */

#include <stdlib.h>

static seq_t *cache     = NULL;   /* dynamically allocated cache of terms   */
static idx_t  cache_cap = 0;      /* number of elements allocated           */
static idx_t  last_computed = 0;  /* highest index that has been computed   */

/* Helper: resize the cache array to accommodate indices < new_cap */
static void ensure_cache_cap(idx_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t ncap = (size_t)new_cap;

#ifdef __cplusplus
    /* Allocate new array (C++ style) */
    seq_t *tmp = new seq_t[ncap];
    for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];   /* copy old */
    delete [] cache;                                            /* delete old */
    cache = tmp;
    cache_cap = (idx_t)ncap;
#else
    /* Allocate new array using realloc (C style) */
    seq_t *tmp = (seq_t *)realloc(cache,
                                    ncap * sizeof(seq_t));
    cache = tmp;
    cache_cap = (idx_t)ncap;
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for indices < 1 */

    /* Ensure cache large enough for indices up to n */
    ensure_cache_cap(n + 1);        /* index 0 unused -> size n+1 */

    /* Initialise first two terms once */
    if (last_computed == 0) {
        /* Ensure space for indices 1 and 2 */
        ensure_cache_cap(3);
        cache[1] = (seq_t)182;
        cache[2] = (seq_t)264446;
        last_computed = 2;
    }

    /* If value already computed, just return it */
    if (n <= last_computed) return cache[n];

    /* Compute remaining terms up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)1454 * cache[i-1] - cache[i-2];
    }
    last_computed = n;

    return cache[n];
}
