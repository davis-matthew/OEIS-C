/*
 * A177180  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=10, k=1 and l=-1.
 * https://oeis.org/A177180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=10, a(n+1)=∑_{p=0}^{n} a(p)*a(n-p)+n  (for n≥1) */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;   /* array of computed a(i) */
static idx_t cache_len = 0;   /* number of valid entries in cache */

static void cache_ensure(idx_t n)
{
    if (n < cache_len) return;                /* already computed */

    /* grow array to hold indices up to n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache = tmp;

    /* initialise new entries to zero */
    for (idx_t i = cache_len; i <= n; ++i)
        cache[i] = (seq_t)0;

    /* update cache length */
    cache_len = n + 1;

    /* initialise the seed values if they are still zero */
    if (cache_len > 0 && cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;
    if (cache_len > 1 && cache[1] == (seq_t)0)
        cache[1] = (seq_t)10;
}

/* compute a(n) recursively with memoisation */
seq_t a(idx_t n)
{
    cache_ensure(n);

    if (n <= 1) return cache[n];            /* base values already set */

    /* compute missing values up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;      /* already computed */

        /* convolution sum for index i: sum_{p=0}^{i-1} a[p]*a[i-1-p] */
        idx_t m = i - 1;
        seq_t conv = (seq_t)0;
        for (idx_t p = 0; p <= m / 2; ++p) {
            idx_t q = m - p;
            if (p < q)
                conv += cache[p] * cache[q] * (seq_t)2;
            else /* p == q */
                conv += cache[p] * cache[q];
        }
        /* add linear part (i-1) */
        cache[i] = conv + (seq_t)(i - 1);
    }

    return cache[n];
}
