/*
 * A041665  Denominators of continued fraction convergents to sqrt(351).
 * https://oeis.org/A041665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A041665: denominators of continued fraction convergents to sqrt(351)
 * Recurrence: a(n) = 124850*a(n-12) - a(n-24)   for n > 23
 * Initial values a(0)..a(23) are taken from the definition.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache that remembers all already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of the cache array */
    static idx_t  cache_len  = 0;   /* number of terms computed (max index + 1) */

    /* initial 24 values of the sequence */
    static const seq_t init24[24] = {
        1ULL,    1ULL,    3ULL,    4ULL,   15ULL,   34ULL,   83ULL,  200ULL,
        683ULL,  883ULL, 2449ULL, 3332ULL,122401ULL,125733ULL,373867ULL,499600ULL,
        1872667ULL,4244934ULL,10362535ULL,24970004ULL,85272547ULL,110242551ULL,
        305757649ULL,416000200ULL
    };

    /* if cache is unexplored, allocate the minimal required space */
    if (cache == NULL) {
        size_t needed = (n + 1 > 24) ? (size_t)n + 1 : 24;
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failed – produce 0 */
        for (size_t i = 0; i < 24; ++i) cache[i] = init24[i];
        cache_cap = needed;
        cache_len  = 24;
    }
    /* grow the cache if the caller asked for a larger index */
    else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1 > cache_cap * 2) ? (size_t)n + 1 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 24) {
            /* values already set in init24 */
            continue;
        }
        cache[i] = (seq_t)124850 * cache[i-12] - cache[i-24];
    }
    cache_len = n + 1;

    return cache[n];
}
