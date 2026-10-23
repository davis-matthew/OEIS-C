/*
 * A049686  a(n) = Fibonacci(8n)/3.
 * https://oeis.org/A049686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 47*a(n-1) - a(n-2), with a(0)=0, a(1)=7
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache with dynamic size */
    static seq_t *cache    = NULL;      /* array of computed values */
    static idx_t cache_sz  = 0;          /* current allocated size */

    /* if the requested index is already computed, return it immediately */
    if (n < cache_sz)
        return cache[n];

    /* enlarge the cache to accommodate index n */
    idx_t old_sz = cache_sz;
    idx_t new_sz = (old_sz == 0) ? 1 : old_sz;
    while (new_sz <= n)
        new_sz *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed; abort gracefully */
        abort();
    }

    /* zero-initialize the newly allocated part */
    for (idx_t i = old_sz; i < new_sz; ++i)
        tmp[i] = 0;

    cache = tmp;

    /* set initial values if missing */
    if (old_sz <= 0 && new_sz > 0)
        cache[0] = 0;
    if (old_sz <= 1 && new_sz > 1)
        cache[1] = 7;

    /* compute any new entries */
    idx_t start = (old_sz > 2) ? old_sz : 2;
    for (idx_t i = start; i < new_sz; ++i) {
        cache[i] = (seq_t)47 * cache[i - 1] - cache[i - 2];
    }

    cache_sz = new_sz;
    return cache[n];
}
