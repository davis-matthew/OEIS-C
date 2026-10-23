/*
 * A013170  Expansion of e.g.f.: exp(arctanh(x)+arcsin(x)).
 * https://oeis.org/A013170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2-3*n+5)*a(n-2)+(n^2-6*n+10)*a(n-3) Initial values:
 *            a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, abort */

static seq_t *cache = NULL;        /* memoization array */
static size_t cache_size = 0;      /* number of stored values */

/* Ensure that cache contains values up to and including index n. */
static void grow_cache(idx_t n)
{
    if ((idx_t)cache_size > n)   /* already enough */
        return;

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)                 /* allocation failed */
        abort();
    cache = new_cache;

    /* compute missing values */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        if (i == 0)
            cache[0] = 1;
        else if (i == 1)
            cache[1] = 2;
        else if (i == 2)
            cache[2] = 4;
        else {
            /* coefficients: (i^2-3i+5) and (i^2-6i+10) */
            idx_t ii = i * i;                          /* i^2, safe for given ranges */
            seq_t c1 = (seq_t)(ii) - (seq_t)(3 * i) + (seq_t)5;
            seq_t c2 = (seq_t)(ii) - (seq_t)(6 * i) + (seq_t)10;
            seq_t term1 = c1 * cache[i - 2];
            seq_t term2 = c2 * cache[i - 3];
            cache[i] = term1 + term2;
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    grow_cache(n);
    return cache[(size_t)n];
}
