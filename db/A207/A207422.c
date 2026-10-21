/*
 * A207422  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207422 – Number of n×4 0..1 arrays avoiding
   the forbidden patterns.
   Recurrence (valid for n ≥ 16):
       a(n) = 4*a(n-1) + a(n-2) + 8*a(n-3) - 21*a(n-4)
              - 46*a(n-5) - 6*a(n-6) + 35*a(n-7) + 49*a(n-8)
              + 19*a(n-9) - 16*a(n-10) - 21*a(n-11)
              - 4*a(n-13) - 2*a(n-14) + a(n-15)
   Initial values:
       a(1) = 9,  a(2) = 81,  a(3) = 361,  a(4) = 1521,
       a(5) = 6889, a(6) = 29929, a(7) = 127449, a(8) = 546121,
       a(9) = 2337841, a(10) = 9979281, a(11) = 42601729,
       a(12) = 181899169, a(13) = 776458225, a(14) = 3314189761,
       a(15) = 14146485721
*/
#include <stdlib.h>

static const seq_t init_vals[16] = {
    (seq_t)0,                            /* a(0) unused */
    (seq_t)9, (seq_t)81, (seq_t)361, (seq_t)1521,
    (seq_t)6889, (seq_t)29929, (seq_t)127449,
    (seq_t)546121, (seq_t)2337841, (seq_t)9979281,
    (seq_t)42601729, (seq_t)181899169, (seq_t)776458225,
    (seq_t)3314189761ULL, (seq_t)14146485721ULL
};

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity (≥ cache_size + 1) */
static size_t cache_size = 0;  /* highest index that has been computed */

/* initialise the cache with the first 15 values */
static void init_cache(void)
{
    if (cache) return;                     /* already initialised */

    /* make sure we have at least capacity 16 (indexes 0–15) */
    size_t needed = 16;
    cache_cap = needed;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();                   /* allocation failure */

    /* load the known values (a[0] is unused) */
    for (size_t i = 1; i < 16; ++i)
        cache[i] = init_vals[i];
    cache_size = 15;
}

/* ensure the cache can hold at least indices up to 'need' (inclusive) */
static void ensure_capacity(size_t need)
{
    if (need < cache_cap)
        return;                            /* already enough */

    /* grow until we have at least 'need' slots */
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= need)
        new_cap *= 2;

    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) abort();                      /* reallocation failure */
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 */
    size_t idx = (size_t)n;

    /* lazily initialise the cache */
    if (!cache)
        init_cache();

    /* if the result is already cached, return it directly */
    if (idx <= cache_size)
        return cache[idx];

    /* extend the cache to cover the requested index */
    ensure_capacity(idx);

    /* compute missing terms sequentially */
    for (size_t k = cache_size + 1; k <= idx; ++k)
    {
        seq_t val = 4 * cache[k - 1] + cache[k - 2] + 8 * cache[k - 3]
                  - 21 * cache[k - 4] - 46 * cache[k - 5] - 6 * cache[k - 6]
                  + 35 * cache[k - 7] + 49 * cache[k - 8] + 19 * cache[k - 9]
                  - 16 * cache[k - 10] - 21 * cache[k - 11]
                  - 4 * cache[k - 13] - 2 * cache[k - 14] + cache[k - 15];
        cache[k] = val;
    }

    cache_size = idx;
    return cache[idx];
}
