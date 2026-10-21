/*
 * A202835  Expansion of e.g.f.: exp(9*x/(1-2*x)) / sqrt(1-4*x^2).
 * https://oeis.org/A202835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+7)*a(n-1) + 2*(n-1)*(2*n+7)*a(n-2) -
 *            8*(n-1)*(n-2)^2*a(n-3); Initial values: a(0)=1, a(1)=9, a(2)=121
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array of computed (n+1) values */
static size_t   cache_cap = 0;     /* current allocation capacity (in seq_t elements) */
static idx_t    cache_len = 0;     /* number of values actually stored */

/* Grow the cache to at least n+1 elements and compute any missing terms. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* First time: allocate a small buffer and fill base cases. */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)121;
        cache_len = 3;
    }

    /* Expand capacity if necessary. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Compute terms up to n, storing them in the cache. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term1 = (seq_t)(2 * i + 7) * cache[i - 1];
        seq_t term2 = (seq_t)2 * (seq_t)(i - 1) * (seq_t)(2 * i + 7) * cache[i - 2];
        seq_t term3 = (seq_t)8 * (seq_t)(i - 1) * (seq_t)(i - 2) *
                      (seq_t)(i - 2) * cache[i - 3];
        seq_t val = term1 + term2;
        val -= term3;           /* subtraction is safe: the recurrence guarantees positivity */
        cache[i] = val;
    }
    cache_len = n + 1;
}

/* exported sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
