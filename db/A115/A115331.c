/*
 * A115331  Expansion of e.g.f. exp(x+5/2*x^2).
 * https://oeis.org/A115331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5*(n-1)*a(n-2) (a(0) = a(1) = 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed a(n) values          */
static size_t cache_len = 0;  /* number of elements actually computed   */
static size_t cache_cap = 0;  /* current allocated capacity             */

seq_t a(idx_t n)
{
    /* initialise first two terms */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure */
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }

    /* return cached value if available */
    if (n < cache_len)
        return cache[n];

    /* grow the array capacity if necessary */
    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;                     /* double until big enough */
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t term = (seq_t)5 * (seq_t)(i - 1) * prev2;
        cache[i] = prev + term;
    }
    cache_len = n + 1;                      /* all values up to n are now stored */

    return cache[n];
}
