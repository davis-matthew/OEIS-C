/*
 * A133209  a(n) = 4a(n-1) - 6a(n-2) + 4a(n-3), n > 3; a(0) = 3, a(1) = 2,
 *          a(2) = a(3) = 0.
 * https://oeis.org/A133209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(1)=2, a(2)=a(3)=0; for n>3,
 *            a(n)=4a(n-1)-6a(n-2)+4a(n-3)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache      = NULL; /* Cached values of the sequence */
static idx_t cache_len   = 0;    /* Number of cached terms (highest index + 1)   */
static size_t cache_cap  = 0;    /* Allocated capacity of the cache              */

static void ensure_cache(idx_t upto)
{
    /* Allocate and initialise the cache if it is empty. */
    if (cache == NULL) {
        size_t cap = 8;
        while (cap <= (size_t)upto)
            cap <<= 1;                         /* make a power‑of‑two size */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache_cap = cap;
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache_len = 4;                         /* we now have indices 0..3 */
    }
    /* Grow the cache if we are asked for a larger index. */
    else if (cache_cap <= (size_t)upto) {
        size_t newcap = cache_cap;
        while (newcap <= (size_t)upto)
            newcap <<= 1;
        seq_t *newcache = (seq_t *)malloc(newcap * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; ++i)
            newcache[i] = cache[i];
        free(cache);
        cache = newcache;
        cache_cap = newcap;
    }

    /* Compute any missing terms up to 'upto'. */
    if (cache_len <= upto) {
        for (idx_t i = cache_len; i <= upto; ++i) {
            cache[i] = (seq_t)4 * cache[i-1]
                     - (seq_t)6 * cache[i-2]
                     + (seq_t)4 * cache[i-3];
        }
        cache_len = upto + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
