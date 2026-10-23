/*
 * A166915  a(n) = 20*a(n-1) - 64*a(n-2) - 45 for n>1; a(0) = 399, a(1) =
 *          5695.
 * https://oeis.org/A166915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 64*a(n-2) - 45 for n>1; a(0) = 399, a(1) =
 *            5695
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <string.h>   /* memcpy */

seq_t a(idx_t n)
{
    /* Static cache that works for both C and C++ builds.
       In C a simple malloc/realloc pair is used; in C++ a
       dynamic array (new/delete[]) is used so that constructors
       are called correctly. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of valid entries in cache */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Handle the two tiny initial values directly. */
    if (n == 0)
        return (seq_t)399;
    if (n == 1)
        return (seq_t)5695;

    /* First time we are called, or cache is empty: allocate. */
    if (cache_len == 0) {
        /* Ensure at least two slots for a(0) and a(1).  We also
           allocate a few extra slots to reduce the number of
           reallocations for typical usage. */
        idx_t init_cap = 2;
        /* If the request is larger than 2, start with that size. */
        if (n + 1 > init_cap)
            init_cap = n + 1;
#ifdef __cplusplus
        cache = new seq_t[init_cap];
#else
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
#endif
        if (!cache) {          /* allocation failed – vector fallback */
            return (seq_t)0;
        }
        cache_cap = init_cap;
        cache[0] = (seq_t)399;
        cache[1] = (seq_t)5695;
        cache_len = 2;
    }

    /* Grow the cache if needed. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        /* copy previously computed values */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        delete [] cache;
#else
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {          /* reallocation failed */
            return (seq_t)0;
        }
        new_cache = cache; /* reuse the same pointer for memcpy if needed */
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If we already have the requested value, return it. */
    if (n < cache_len)
        return cache[n];

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t val = 20 * a1;      /* 20*a(n-1) */
        val -= 64 * a2;           /* -64*a(n-2) */
        val -= (seq_t)45;         /* -45 */
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
