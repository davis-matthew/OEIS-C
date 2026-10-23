/*
 * A026067  a(n) = (d(n)-r(n))/2, where d = A026066 and r is the periodic
 *          sequence with fundamental period (1,0,0,0).
 * https://oeis.org/A026067
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   free_seq_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (d(n)-r(n))/2,  d = A026066, r has period (1,0,0,0)
 * Recurrence derived from generating function:
 *   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4) - 3*a(n-5) + 3*a(n-6) - a(n-7)
 *   valid for all n >= 14, with initial values a(7)=52, a(8)=84, ..., a(13)=364
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/free_seq_arr.h"

/* Forward declaration of helper that allocates an array of seq_t */
static seq_t* alloc_seq_arr(size_t n);

static seq_t* alloc_seq_arr(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n]();
#else
    return (seq_t*)calloc(n, sizeof(seq_t));
#endif
}

/* static cache */
static seq_t *cache      = NULL;
static size_t cacheSize  = 0;      /* allocated size (elements 0 .. cacheSize-1) */
static size_t maxCached  = 0;      /* highest n whose value is already computed */

/* Ensure cache is large enough to hold index n */
static void expand_cache(size_t need)
{
    if (need <= cacheSize)
        return;
    seq_t *newCache = alloc_seq_arr(need);
    if (!newCache)
        return;   /* allocation failure – silent; caller will return 0 */
    for (size_t i = 0; i < cacheSize; ++i)
        newCache[i] = cache[i];
    free_seq_arr(cache);
    cache      = newCache;
    cacheSize  = need;
}

/* Handle first-time allocation and seed initial values */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    /* Allocate space up to index 13 (inclusive) */
    cacheSize = 14;      /* indices 0..13 */
    cache = alloc_seq_arr(cacheSize);
    if (!cache)
        return;   /* allocation failure – silent */
    /* Seed known values a(7) .. a(13) */
    cache[7]  = 52;
    cache[8]  = 84;
    cache[9]  = 123;
    cache[10] = 170;
    cache[11] = 225;
    cache[12] = 290;
    cache[13] = 364;
    maxCached = 13;      /* last computed index */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 7)                       /* undefined below offset – return 0 */
        return (seq_t)0;

    init_cache();
    if (n + 1 > cacheSize)
        expand_cache((size_t)n + 1);
    if (n > (idx_t)maxCached)
    {
        size_t start = maxCached + 1;
        size_t end   = (size_t)n;
        for (size_t i = start; i <= end; ++i)
        {
            cache[i] =  3 * cache[i-1]
                       - 3 * cache[i-2]
                       +   cache[i-3]
                       +   cache[i-4]
                       - 3 * cache[i-5]
                       + 3 * cache[i-6]
                       -   cache[i-7];
        }
        maxCached = n;
    }
    return cache[(size_t)n];
}
