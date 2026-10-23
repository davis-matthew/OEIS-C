/*
 * A133710  Column l=3 of irregular triangle in A133709.
 * https://oeis.org/A133710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) for n > 7 Initial values: a(1)=0, a(2)=3,
 *            a(3)=35, a(4)=131, a(5)=347, a(6)=767, a(7)=1511
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[8] = {0, 0, 3, 35, 131, 347, 767, 1511};
static seq_t *cache = NULL;          /* 1‑based array of cached terms */
static size_t cache_cap = 0;         /* current capacity of cache array    */
static idx_t  last_computed = 0;     /* highest index that has been computed */

/* Ensure the cache can hold indices up to at least 'needed' (1‑based).   */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t newcap = cache_cap ? cache_cap << 1 : 16;
    while (newcap < needed)
        newcap <<= 1;

    seq_t *newcache = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newcache) {
        /* Terminate: not enough memory.  The OEIS harness will treat this as a failure. */
        exit(1);
    }

    if (cache) {
        for (size_t i = 1; i <= cache_cap; ++i)
            newcache[i] = cache[i];
        free(cache);
    }
    else {
        /* Uninitialized for first time. No need to clear, but zero used elements for safety. */
        for (size_t i = 1; i < newcap; ++i)
            newcache[i] = 0;
    }

    cache      = newcache;
    cache_cap  = newcap;
}

seq_t a(idx_t n)
{
    if (n <= 7)
        return init_vals[(size_t)n];

    /* Expand cache if necessary. */
    grow_cache((size_t)n + 1);

    /* Initialise the cache for the first seven terms once. */
    if (last_computed < 7) {
        for (size_t i = 1; i <= 7; ++i)
            cache[i] = init_vals[i];
        last_computed = 7;
    }

    /* Compute any missing terms up to n. */
    while (last_computed < n) {
        /* All indices below 1 are never accessed because last_computed >= 7 here. */
        seq_t val;

        val = 7 * cache[last_computed];                 /* 7*a(n-1) */
        val -= 21 * cache[last_computed - 1];           /* -21*a(n-2) */
        val += 35 * cache[last_computed - 2];           /* +35*a(n-3) */
        val -= 35 * cache[last_computed - 3];           /* -35*a(n-4) */
        val += 21 * cache[last_computed - 4];           /* +21*a(n-5) */
        val -= 7  * cache[last_computed - 5];           /* -7*a(n-6) */
        val +=      cache[last_computed - 6];           /* +a(n-7)   */

        ++last_computed;
        cache[last_computed] = val;
    }

    return cache[n];
}
