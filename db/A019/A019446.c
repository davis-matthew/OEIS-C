/*
 * A019446  a(n) = ceiling(n/tau), where tau = (1+sqrt(5))/2.
 * https://oeis.org/A019446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n + 1 - a(a(n-1)), a(1) = 1 (OEIS A019446)
 * Range:     u8 n <= 412, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache that keeps all computed values */
static seq_t *cache = NULL;      /* array of a(i) */
static idx_t cache_size = 0;     /* number of valid entries (max index stored + 1) */

/* helper to ensure the cache can hold at least indices up to n */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    idx_t new_size = cache_size ? cache_size : 1;
    while (new_size <= n)
        new_size *= 2;

    /* allocate or grow the cache */
    seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (!tmp) {
        /* memory allocation failed – abort gracefully */
        exit(EXIT_FAILURE);
    }

    /* initialize new entries to 0 */
    if (tmp != cache) {
        /* existing part preserved; new part zeroed */
        for (idx_t i = cache_size; i <= new_size; ++i)
            tmp[i] = (seq_t)0;
    } else if (new_size > cache_size) {
        for (idx_t i = cache_size; i <= new_size; ++i)
            tmp[i] = (seq_t)0;
    }

    cache = tmp;
    cache_size = new_size + 1; /* we use indices from 0 to new_size inclusive */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n < 1 – return 0 */

    ensure_cache(n);

    /* initialise first term */
    if (!cache[1])
        cache[1] = (seq_t)1;

    /* compute missing terms up to n (forward only) */
    for (idx_t i = 2; i <= n; ++i) {
        if (!cache[i]) {
            seq_t prev_val = cache[i - 1];
            idx_t inner_idx = (idx_t)prev_val;
            seq_t inner_val = cache[inner_idx];
            cache[i] = (seq_t)(i + 1) - inner_val;
        }
    }

    return cache[n];
}
