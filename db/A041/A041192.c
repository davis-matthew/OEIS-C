/*
 * A041192  Numerators of continued fraction convergents to sqrt(107).
 * https://oeis.org/A041192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 1924*a(n-6) - a(n-12) Initial values (n = 0..11): 10, 21,
 *            31, 300, 331, 962, 19571, 40104, 59675, 577179, 636854, 1850887
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for arbitrary call order */
    static seq_t *cache      = NULL;   /* computed terms   */
    static size_t cache_len  = 0;      /* number of entries in cache */
    static size_t cache_cap  = 0;      /* allocated capacity       */

    /* Initial 12 terms – required to start the recurrence */
    static const seq_t init[12] = {
        10, 21, 31, 300, 331, 962,
        19571, 40104, 59675, 577179,
        636854, 1850887
    };

    size_t idx = (size_t)n;            /* safe cast for indexing within limits */

    /* Return early for indices less than 12 */
    if (n < 12) {
        return init[idx];
    }

    /* Allocate cache on first use */
    if (cache == NULL) {
        /* Ensure capacity at least (n+1) and >=12 */
        cache_cap = idx + 1;
        if (cache_cap < 12) cache_cap = 12;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure: return 0 */
        for (size_t i = 0; i < 12; ++i) {
            cache[i] = init[i];
        }
        cache_len = 12;
    }

    /* Grow cache if needed */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 12;
        while (new_cap <= idx) {
            new_cap <<= 1;             /* double until enough */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;     /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= idx; ++i) {
        cache[i] = (seq_t)1924 * cache[i - 6] - cache[i - 12];
    }
    cache_len = idx + 1;

    return cache[idx];
}
