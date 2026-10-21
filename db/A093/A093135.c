/*
 * A093135  Expansion of g.f. (1-8*x)/((1-x)*(1-10*x)).
 * https://oeis.org/A093135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*10^n + 7)/9 (equivalently a(n) = 10*a(n-1) - 7,
 *            a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* memoized terms */
static idx_t cache_size = 0;      /* number of cached terms */

seq_t a(idx_t n)
{
    /* Allocate / grow cache to hold term n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) return (seq_t)0;   /* allocation failed */
        cache = tmp;

        /* Ensure base values are known */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;       /* a(0) */
            if (new_size > 1) cache[1] = (seq_t)3; /* a(1) */
            cache_size = 2;
        } else if (cache_size == 1) {
            cache[1] = (seq_t)3;       /* a(1) */
            cache_size = 2;
        }

        /* Compute any missing terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t v = cache[i-1];
            v = v * (seq_t)10;   /* 10*a(i-1) */
            v = v - (seq_t)7;    /* 10*a(i-1) - 7 */
            cache[i] = v;
        }
        cache_size = new_size;
    }
    return cache[n];
}
