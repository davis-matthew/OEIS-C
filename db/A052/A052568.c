/*
 * A052568  E.g.f.: (1-x)/(1-3*x+x^2).
 * https://oeis.org/A052568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(1) = 2; * a(n+2) = (3*n+6)*a(n+1) - (n^2+3*n+2)*a(n)
 *            (n >= 0) * (recurrence from OEIS A052568)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values *an array of computed a(n).  */
static seq_t *cache = NULL;      /* pointer to cache array */
static idx_t cache_cap = 0;      /* allocated capacity in elements */
static idx_t cache_valid = 0;    /* number of initially assigned elements */

/* Grow the cache so that it contains a(0)…a(n).  */
static void ensure_cache(idx_t n)
{
    /* Initialisation: store a(0) and a(1) */
    if (cache_valid == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_valid = 2;                /* we have a(0) and a(1) */
    }

    if (n < cache_valid)
        return;                         /* already computed */

    /* Expand the allocated file if required (doubling strategy). */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap <= n)
            new_cap <<= 1;              /* double until we have enough space */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the missing terms by the recurrence.  */
    for (idx_t i = cache_valid; i <= n; ++i) {
        /* i == 0 or 1 would have been handled by initialisation.  */
        idx_t m = i - 2;                /* n in the recurrence  a(n+2) */
        seq_t term1 = (seq_t)(3 * m + 6) * cache[i - 1];
        seq_t term2 = (seq_t)(m * m + 3 * m + 2) * cache[i - 2];
        cache[i] = term1 - term2;
    }
    cache_valid = n + 1;                  /* we now have a(0)…a(n) */
}

/* Public function requested by the test harness.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
