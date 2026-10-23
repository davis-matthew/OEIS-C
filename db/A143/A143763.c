/*
 * A143763  a(n+1) = (a(n)-n)^2, a(1) = 1.
 * https://oeis.org/A143763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n+1) = (a(n) - n)^2 (OEIS offset 1)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of computed values: cache[1] = a(1), ... */
static seq_t *cache = NULL;
static idx_t cache_up_to = 0;  /* inclusive highest computed index */

/* extend the cache to at least index m */
static void compute_up_to(idx_t m)
{
    if (cache == NULL) {
        size_t sz = (size_t)(m + 1);   /* need indices 0..m, index 0 unused */
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        cache[0] = (seq_t)0;          /* unused */
        cache[1] = (seq_t)1;          /* a(1) */
        cache_up_to = 1;
    }
    if (m <= cache_up_to) return;    /* already computed */

    /* extend storage */
    size_t newsize = (size_t)(m + 1);
    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp) abort();               /* out of memory */
    cache = tmp;

    /* compute new entries */
    for (idx_t i = cache_up_to + 1; i <= m; ++i) {
        seq_t prev = cache[i - 1];
        seq_t step = (seq_t)(i - 1);            /* n in the recurrence */
        seq_t diff = (prev >= step) ? prev - step : step - prev;
        cache[i] = diff * diff;
    }
    cache_up_to = m;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* sequence defined for n >= 1 */
    compute_up_to(n);
    return cache[n];
}
