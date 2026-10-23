/*
 * A176964  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=3, k=-1 and l=1.
 * https://oeis.org/A176964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(n+1)=∑_{p=0}^{n} (a(p)*a(n-p)+k) + l, with
 *            k=-1, l=1 -> a(n+1)=∑_{p=0}^{n} a(p)*a(n-p) – n (n≥1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Utility: grow cache to at least 'min_cap' elements */
static void
ensure_cache(idx_t min_cap)
{
    if (cache_size >= min_cap)
        return;

    /* Double the size until big enough, but at least 2 */
    idx_t new_cap = cache_size ? cache_size * 2 : 2;
    while (new_cap < min_cap)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed; in this competition setting we abort */
        abort();
    }
    cache = new_cache;
    cache_size = new_cap;
}

/* Compute a(n) using memoized recurrence */
seq_t a(idx_t n)
{
    /* Ensure we have at least n+1 entries */
    ensure_cache(n + 1);

    /* Initialize base cases on first run */
    if (cache_size >= 2 && cache[0] == 0 && cache[1] == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    }

    /* Compute missing terms up to n */
    for (idx_t m = 2; m <= n; ++m) {
        /* m is the index of the term to compute (a[m]) */
        /* sum over p=0..m-1 of a[p]*a[m-1-p] */
        seq_t sum = 0;
        for (idx_t p = 0; p < m; ++p) {
            sum += cache[p] * cache[m - 1 - p];
        }
        /* subtract (m-1) because of k = -1 in recurrence */
        sum -= (seq_t)(m - 1);
        /* store result */
        cache[m] = sum;
    }

    return cache[n];
}
