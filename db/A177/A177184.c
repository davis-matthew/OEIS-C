/*
 * A177184  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=9, k=-1 and l=-1.
 * https://oeis.org/A177184
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

/* Recurrence: a(0)=1, a(1)=9, for n>=2
   a(n) = Σ_{p=0}^{n-1} a(p) * a(n-1-p)  - (n+1) */
#include <stdlib.h>
#include <stddef.h>

/* Cached sequence values a(0)...a(n) in increasing order */
static seq_t *cache = NULL;
static idx_t cache_cap = 0; /* allocated size */
static idx_t cache_len = 0; /* smallest n for which cache[n] is valid */

/* Ensure that cache has at least 'need' entries, reallocating if required. */
static void ensure_cache(idx_t need)
{
    if (need < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap <= need)
        new_cap <<= 1;
    /* Reallocate with correct element size. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Memoised sequence computation. */
seq_t a(idx_t n)
{
    /* Initialise the cache if necessary. */
    if (cache_len == 0) {
        ensure_cache(8);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache_len = 2;
    }

    /* Return cached value if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to accommodate all needed indices. */
    ensure_cache(n);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t p = 0; p < i; ++p)
            sum += cache[p] * cache[i - 1 - p];
        sum -= ((seq_t)i + 1);           /* subtract (i+1) */
        cache[i] = sum;
    }
    cache_len = n + 1;
    return cache[n];
}
