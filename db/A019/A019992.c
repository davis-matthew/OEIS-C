/*
 * A019992  a(n) = 4*a(n-1) + a(n-2) - a(n-3) - a(n-5).
 * https://oeis.org/A019992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - a(n-3) - a(n-5) for n>=5 Initial
 *            values: a(0)=5, a(1)=21, a(2)=88, a(3)=368, a(4)=1538, a(5)=6427
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values */
static idx_t  cache_len = 0;     /* number of values stored (highest index+1) */
static size_t cache_cap = 0;     /* allocated capacity of cache [] */

/* Ensure the cache can store indices up to n (inclusive).  If the cache
   currently has smaller capacity, grow it. */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;                 /* already enough */
    /* grow to at least n+1, doubled each time for amortized O(1) */
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Returning a(n) for any n >= 0; repeated calls are cheap due to memoisation */
seq_t a(idx_t n)
{
    /* Base cases: index < 5 are handled directly by the cache after initialization */
    /* initialise cache on first call */
    if (cache == NULL) {
        ensure_capacity(5);              /* need indices 0..5 */
        cache[0] = 5;
        cache[1] = 21;
        cache[2] = 88;
        cache[3] = 368;
        cache[4] = 1538;
        cache[5] = 6427;
        cache_len = 6;                   /* indices 0..5 present */
    }

    /* for n < 5 simply return the cached value */
    if (n < 5)
        return cache[n];

    /* extend cache up to n if necessary */
    if (n + 1 > cache_len) {
        ensure_capacity(n);              /* allocate enough space */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(i) = 4*a(i-1) + a(i-2) - a(i-3) - a(i-5) */
            seq_t val = (seq_t)4 * cache[i - 1] + cache[i - 2]
                        - cache[i - 3] - cache[i - 5];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
