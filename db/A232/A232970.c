/*
 * A232970  Expansion of (1-3*x)/(1-5*x+3*x^2+x^3).
 * https://oeis.org/A232970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=7, a(n)=5*a(n-1)-3*a(n-2)-a(n-3) for n≥3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoization array */
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t cache_max = 0;    /* highest n computed so far */

seq_t a(idx_t n)
{
    /* initialise cache with first three terms */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache_max = 2;
    }

    /* return cached value if available */
    if (n <= cache_max) {
        return cache[n];
    }

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values using recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = 5 * cache[i - 1];
        cache[i] -= 3 * cache[i - 2];
        cache[i] -= cache[i - 3];
    }
    cache_max = n;
    return cache[n];
}
