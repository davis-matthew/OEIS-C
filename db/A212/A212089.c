/*
 * A212089  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          w>=average{x,y,z}.
 * https://oeis.org/A212089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 5*a(n-4) + 6*a(n-5) -
 *            4*a(n-6) + a(n-7). Initial values: a(0)=0, a(1)=1, a(2)=9,
 *            a(3)=45, a(4)=139, a(5)=333, a(6)=684
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h> /* for memcpy */

seq_t a(idx_t n)
{
    /* static cache to keep computed values */
    static seq_t *cache = NULL;         /* array of values */
    static size_t cache_len = 0;        /* number of valid entries */
    static size_t cache_cap = 0;        /* allocated capacity */

    /* helper to make sure the cache can hold indices up to 'need' (inclusive) */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure – aborting cleanly, value undefined */
            return (seq_t)0;
        }
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* initialise the first 7 values on first use or if cache was just grown below 7 */
    if (cache_len < 7) {
        const seq_t seed[7] = { (seq_t)0, (seq_t)1, (seq_t)9,
                                (seq_t)45, (seq_t)139, (seq_t)333, (seq_t)684 };
        size_t i;
        for (i = cache_len; i < 7 && i < cache_cap; ++i)
            cache[i] = seed[i];
        if (i > cache_len) cache_len = i;
    }

    /* compute any missing terms up to n */
    if ((size_t)n >= cache_len) {
        size_t i;
        for (i = cache_len; (size_t)i <= (size_t)n; ++i) {
            /* recurrence requires i >= 7, but by construction
               cache_len >= 7 before this loop runs (seeded above) */
            seq_t val = (seq_t)4 * cache[i-1];
            val -= (seq_t)6 * cache[i-2];
            val += (seq_t)5 * cache[i-3];
            val -= (seq_t)5 * cache[i-4];
            val += (seq_t)6 * cache[i-5];
            val -= (seq_t)4 * cache[i-6];
            val += cache[i-7];
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
