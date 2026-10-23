/*
 * A116090  Expansion of 1/(1-x^2*(1+x)^3).
 * https://oeis.org/A116090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + 3*a(n-3) + 3*a(n-4) + a(n-5) with a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=3, a(4)=4
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 71, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_cap = 0;     /* current allocated size (≥ number of cached terms) */
static idx_t   max_computed = 4; /* largest index for which a(k) is known */

/* initialise the first five cache entries when the buffer is large enough */
static void init_cache(size_t cap)
{
    if (cap >= 1) cache[0] = 1;
    if (cap >= 2) cache[1] = 0;
    if (cap >= 3) cache[2] = 1;
    if (cap >= 4) cache[3] = 3;
    if (cap >= 5) cache[4] = 4;
}

seq_t a(idx_t n)
{
    /* first five terms are known directly */
    if (n <= 4) {
        static const seq_t init[5] = {1, 0, 1, 3, 4};
        return init[n];
    }

    /* ensure the cache is large enough */
    size_t needed = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        cache_cap = needed;
        init_cache(cache_cap);
    } else if (cache_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) { /* allocation failed – let the program abort naturally */
            return 0;
        }
        cache = tmp;
        cache_cap = needed;
        init_cache(cache_cap);
    }

    /* compute any new terms beyond what is already known */
    if (n > max_computed) {
        size_t start = (size_t)max_computed + 1;
        size_t end   = (size_t)n;
        for (size_t i = start; i <= end; ++i) {
            cache[i] = cache[i-2] + 3 * cache[i-3] + 3 * cache[i-4] + cache[i-5];
        }
        max_computed = n;
    }

    return cache[(size_t)n];
}
