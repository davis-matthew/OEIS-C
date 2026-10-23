/*
 * A168862  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 39*(a(n-1)+a(n-2)+…+a(n-19)) - 780*a(n-20) + N(n) where
 *            N(0)=1, N(1..19)=2, N(20)=1, N(n≥21)=0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;          /* array storing a(0)…a(k)  */
static size_t cache_cap = 0;         /* allocated array capacity in elements */
static idx_t   cached_n   = 0;       /* number of stored terms (cache[0…cached_n-1]) */

/* Allocate a larger cache when needed */
static void ensure_cache_capacity(idx_t n)
{
    /* we need indices up to n inclusive -> size = n+1 */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;               /* double until large enough */

#ifdef __cplusplus
    /* C++ new: default‑construct array elements */
    seq_t *new_cache = new seq_t[new_cap];
#else
    /* C malloc: elements (usually trivially‑constructible) */
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    assert(new_cache && "memory allocation failed");

    /* copy already computed values */
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Main sequence accessor */
seq_t a(idx_t n)
{
    if (n < cached_n)
        return cache[n];

    /* extend cache up to n */
    ensure_cache_capacity(n);

    /* compute missing terms */
    for (idx_t i = cached_n; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = 0;
        idx_t m = (i < 19 ? i : 19);
        for (idx_t j = 1; j <= m; ++j)
            sum += cache[i - j];

        seq_t val = sum * (seq_t)39;           /* 39 * sum_{k=1}^{19} a[i-k] */

        if (i >= 20)
            val -= (seq_t)780 * cache[i - 20]; /* subtract 780 * a[i-20] */

        /* add constant term N(i) */
        if (i <= 19)
            val += 2;          /* N(i)=2 for 1≤i≤19   */
        else if (i == 20)
            val += 1;          /* N(20)=1             */

        cache[i] = val;
    }

    cached_n = n + 1;   /* now we have terms 0…n */
    return cache[n];
}
