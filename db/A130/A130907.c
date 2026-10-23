/*
 * A130907  E.g.f.: exp(x+x^2/2)/(1-x).
 * https://oeis.org/A130907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-1) - (n-2)*(n-1)*a(n-3) with a(0)=1, a(1)=2,
 *            a(2)=6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below memoises all terms up to the largest n requested.
   It uses only the integer arithmetic operations allowed by the
   specification and relies on malloc/realloc/free for storage. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

/* static cache */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed_cnt = 0;

/* make sure cache has room for indices 0..n and that all values
   up to n are computed */
static void ensure_cache(idx_t n)
{
    /* initialise cache if not yet done */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        if (cache_cap < 4) cache_cap = 4;          /* hold at least 0..3 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return;                /* out of memory, will crash later */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        computed_cnt = 3;                         /* we have a(0)..a(2) */
    }

    /* grow cache if needed */
    if ( (size_t)n + 1 > cache_cap ) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return;            /* realloc failed; will crash later */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = computed_cnt; i <= n; ++i) {
        /* term1 = (i+1)*a(i-1) */
        seq_t term1 = (seq_t)(i + 1) * cache[i - 1];

        /* term2 = (i-2)*(i-1)*a(i-3) */
        seq_t mult = (seq_t)(i - 2) * (seq_t)(i - 1);
        seq_t term2 = mult * cache[i - 3];

        cache[i] = term1 - term2;
    }
    computed_cnt = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
