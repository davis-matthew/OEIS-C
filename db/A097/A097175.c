/*
 * A097175  a(n) = Sum_{k=0..n} binomial(floor((n+1)/2), floor((k+1)/2)) *
 *          4^k.
 * https://oeis.org/A097175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(floor((n+1)/2), floor((k+1)/2)) *
 *            4^k Recurrence: a(0)=1, a(1)=5, a(2)=21, a(3)=105, a(4)=361,
 *            a(n)=a(n-1)+33*a(n-2)-33*a(n-3)-272*a(n-4)+272*a(n-5) for n≥5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;    /* array of terms a(0)..a(cache_len-1) */
static idx_t  cache_len = 0;   /* number of terms stored */
static idx_t  cache_cap = 0;   /* allocated capacity */

/* ensure that cache contains at least terms up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initial capacity */
        cache_cap = 16;
        #ifdef __cplusplus
        cache = new seq_t[cache_cap];
        #else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        #endif
        cache_len = 0;
    }

    if (n < cache_len)
        return; /* already have it */

    /* Grow capacity if needed */
    while (cache_cap <= n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= cache_cap) new_cap = n + 1; /* overflow guard */
        #ifdef __cplusplus
        seq_t* new_ptr = new seq_t[new_cap];
        for (idx_t i = 0; i < cache_len; ++i)
            new_ptr[i] = cache[i];
        delete[] cache;
        cache = new_ptr;
        #else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        #endif
        cache_cap = new_cap;
    }

    /* base values */
    const seq_t base[5] = { (seq_t)1, (seq_t)5, (seq_t)21, (seq_t)105, (seq_t)361 };

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 5) {
            cache[i] = base[i];
        } else {
            cache[i] =
                cache[i-1]
              + cache[i-2] * (seq_t)33
              - cache[i-3] * (seq_t)33
              - cache[i-4] * (seq_t)272
              + cache[i-5] * (seq_t)272;
        }
    }
    cache_len = n + 1;
}

/* required function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
