/*
 * A016233  Expansion of 1/((1-x)(1-5x)(1-8x)).
 * https://oeis.org/A016233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 53*a(n-2) + 40*a(n-3); a(0) = 1, a(1) = 14,
 *            a(2) = 143 This recurrence is derived from the generating
 *            function 1/((1-x)(1-5x)(1-8x))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* pointer to stored terms */
static size_t cachelen = 0;      /* number of terms stored */
static size_t cachecap = 0;      /* current capacity of the cache */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold terms up to n */
    if (cachelen <= (size_t)n) {
        /* Grow the cache; start with capacity 4 and double until it fits */
        size_t newcap = cachecap ? cachecap : 4;
        while (newcap <= (size_t)n) newcap <<= 1;

        seq_t *newcache = (seq_t*)malloc(newcap * sizeof(seq_t));
        if (!newcache) { /* allocation failure – return 0 as a fallback */
            return (seq_t)0;
        }

        if (cache) {
            memcpy(newcache, cache, cachelen * sizeof(seq_t));
            free(cache);
        }
        cache     = newcache;
        cachecap  = newcap;

        /* Initialise the very first term if the cache was empty */
        if (cachelen == 0) {
            cache[0] = (seq_t)1;
            cachelen = 1;
        }
    }

    /* Compute any missing terms up to n */
    for (size_t i = cachelen; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;           /* already set but keep here for safety */
        } else if (i == 1) {
            cache[1] = (seq_t)14;
        } else if (i == 2) {
            cache[2] = (seq_t)143;
        } else {
            cache[i] = (14 * cache[i-1]) - (53 * cache[i-2]) + (40 * cache[i-3]);
        }
    }

    cachelen = (size_t)n + 1;
    return cache[(size_t)n];
}
