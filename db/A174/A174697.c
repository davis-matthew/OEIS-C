/*
 * A174697  a(0)=a(1)=a(2)=1, a(n)=a(Floor[a(n-1)/2])+a(n-Floor[a(n-1)/2]).
 * https://oeis.org/A174697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(floor[a(n-1)/2]) + a(n - floor[a(n-1)/2])
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Sequence A174697: a(0)=a(1)=a(2)=a(3)=1, a(4)=2,
// a(n)=a(floor[a(n-1)/2])+a(n-floor[a(n-1)/2]) for n≥5
/* Initial conditions: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=2 */

#include <stdlib.h>

/* External types provided by the build environment */
extern seq_t _seq_placeholder;   /* just to avoid unused type warnings */

/* Memoised computation */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache storage */
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_len = 0;  /* number of filled elements */

    /* Initialise cache with first 5 values */
    if (cache_cap == 0) {
        cache_cap = 8;                          /* start with 8 slots */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failed – unlikely in tests */

        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
        cache[4]                    = (seq_t)2;
        cache_len = 5;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return directly if already computed */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* Compute all missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i <= 4) {                          /* safety for initial terms */
            cache[i] = (seq_t)((i <= 3) ? 1 : 2);
        } else {
            idx_t t = (idx_t)(cache[i - 1] / 2);        /* floor division */
            cache[i] = cache[t] + cache[i - t];
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
