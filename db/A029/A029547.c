/*
 * A029547  Expansion of g.f. 1/(1 - 34*x + x^2).
 * https://oeis.org/A029547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=34, a(n)=34*a(n-1)-a(n-2) for n≥2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache     = NULL;  /* array of computed terms */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t  cache_len = 0;     /* number of terms stored */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first terms if necessary */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 1) {
        cache[1] = (seq_t)34;
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = ((seq_t)34) * cache[i - 1] - cache[i - 2];
    }
    cache_len = n + 1;

    return cache[n];
}
