/*
 * A165324  a(0)=1, a(1)=9, a(n)= 19*a(n-1)-81*a(n-2) for n>1.
 * https://oeis.org/A165324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=9, a(n)=19*a(n-1)-81*a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* portability between C and C++ */
#ifdef __cplusplus
#  include <cstddef>
#endif

/* cache for already computed terms */
static seq_t *cache = NULL;        /* array of cached values          */
static size_t cache_size = 0;      /* number of values currently set   */
static size_t cache_cap  = 0;      /* allocated capacity                */

/* allocate or grow the cache */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    seq_t *newp;
#ifdef __cplusplus
    newp = new seq_t[new_cap];
#else
    newp = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif

    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            newp[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = newp;
    cache_cap = new_cap;
}

/* initialise base values */
static void initialise(void)
{
    if (cache == NULL) {
        ensure_capacity(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache_size = 2;
    }
}

seq_t a(idx_t n)
{
    initialise();

    if ((idx_t)n < cache_size)
        return cache[(size_t)n];

    /* grow cache to cover up to n */
    ensure_capacity((size_t)n + 1);

    /* compute missing terms iteratively */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t prod1 = (seq_t)19 * cache[(size_t)(i-1)];
        seq_t prod2 = (seq_t)81 * cache[(size_t)(i-2)];
        cache[(size_t)i] = prod1 - prod2;
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
