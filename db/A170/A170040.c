/*
 * A170040  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + 29*sum_{k=1}^{35} a(n-k) - 435*a(n-36) where
 *            p(0)=1, p(1..35)=2, p(36)=1, p(n)=0 for n>36
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence computation with caching and O(n) time */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed values          */
static size_t cache_cap = 0;         /* current allocated capacity       */
static size_t cache_len = 0;         /* number of values stored (>=0)    */

/* make sure cache can hold at least need elements */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = need;
    if (new_cap < 16)
        new_cap = 16;
    if (new_cap < cache_cap * 2)
        new_cap = cache_cap * 2;

    if (cache == NULL)
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    cache_cap = new_cap;
}

/* sequence a(n) */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;                /* for indexing into cache */

    if (N >= cache_cap)
        ensure_capacity(N + 1);

    /* compute terms up to N */
    seq_t sum35 = 0;                     /* sum of last up to 35 terms */

    /* If cache already has values, we need to recompute sum35 accordingly */
    if (cache_len > 0) {
        /* Recompute sum35 from end of cached range to N */
        size_t start = cache_len > 35 ? cache_len - 35 : 0;
        sum35 = 0;
        for (size_t i = start; i < cache_len; ++i)
            sum35 += cache[i];
    }

    for (size_t i = cache_len; i <= N; ++i) {
        /* coefficient p(i) */
        seq_t p;
        if (i == 0)
            p = 1;
        else if (i <= 35)
            p = 2;
        else if (i == 36)
            p = 1;
        else
            p = 0;

        /* a[i] = p + 29*sum35 - 435*a[i-36]  */
        seq_t term = p + (seq_t)29 * sum35
                     - (i >= 36 ? (seq_t)435 * cache[i - 36] : (seq_t)0);

        cache[i] = term;

        /* update sum35 for next index */
        if (i < 35) {
            sum35 += term;                       /* still accumulating */
        } else {
            sum35 += term;                       /* add new term          */
            sum35 -= cache[i - 35];              /* drop term leaving window of 35 */
        }
    }

    cache_len = N + 1;
    return cache[N];
}
