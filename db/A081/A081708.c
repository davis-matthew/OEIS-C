/*
 * A081708  a(n) = a(n-1) + 64*a(n-2) starting with a(0) = 2 and a(1) = 1.
 * https://oeis.org/A081708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 970 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081708:
   Recurrence: a(0) = 2, a(1) = 1,
                a(n) = a(n-1) + 64 * a(n-2)   for n ≥ 2 */
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache to store already computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;   /* number of terms stored */

    /* initialise cache with the first two terms */
    if (!cache) {
        cache = mem_new(seq_t, 2);
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* if the requested term is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* grow the cache to accommodate up to index n */
    seq_t *tmp = mem_resize(seq_t, cache, (n + 1));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;

    /* compute the remaining terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i - 1] + (seq_t)64 * cache[i - 2];
    }

    /* update cache length */
    cache_len = n + 1;
    return cache[n];
}
