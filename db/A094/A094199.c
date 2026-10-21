/*
 * A094199  a(0) = -1/2; for n > 0, a(n) = 2*(5*n-4)*(5*n-6)*a(n-1) +
 *          Sum_{k=1..n-1} a(k)*a(n-k).
 * https://oeis.org/A094199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(5*n-4)*(5*n-6)*a(n-1) + sum_{k=1..n-1} a(k)*a(n-k),
 *            a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* static cache for memoisation */
static seq_t *cache  = NULL; /* pointer to array of seq_t  */
static size_t cache_cap = 0; /* current allocated size  */
static idx_t  cache_top  = 0;/* highest index that has been computed (>=1) */

/* Ensure that the values a[1..n] are computed and stored in cache. */
static void ensure_cache(idx_t n)
{
    if (cache_top >= n)
        return;                     /* already computed */

    /* enlarge storage if needed (at least n+1 elements) */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap <= (size_t)(n + 1))
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise first term when called the first time */
    if (cache_top == 0) {
        cache[1] = (seq_t)1;            /* a(1) = 1 */
        cache_top = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_top + 1; i <= n; ++i) {
        /* term from recurrence: 2*(5*i-4)*(5*i-6)*a(i-1) */
        seq_t prod  = (seq_t)2;
        prod *= (seq_t)(5 * i - 4);
        prod *= (seq_t)(5 * i - 6);
        prod *= cache[i - 1];

        /* sum_{k=1..i-1} a(k)*a(i-k) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < i; ++k)
            sum += cache[k] * cache[i - k];

        cache[i] = prod + sum;
    }

    cache_top = n;
}

seq_t a(idx_t n)
{
    assert(n >= 1);
    ensure_cache(n);
    return cache[n];
}
