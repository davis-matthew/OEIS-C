/*
 * A193657  First difference of A002627.
 * https://oeis.org/A193657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)*a(n-1) - (n-1)*a(n-2); a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 183 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized values */
    static idx_t cache_size = 0;  /* number of computed values */

    /* initialise cache for n = 0, 1 */
    if (cache_size == 0) {
        cache_size = 2;
        cache = mem_resize(seq_t, cache, cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
    }

    /* already computed? */
    if (n < cache_size) {
        return cache[n];
    }

    /* grow the cache to accommodate up to index n */
    seq_t *tmp = mem_resize(seq_t, cache, (n + 1));
    if (tmp == NULL) {
        /* allocation failed – in practical use the program will abort */
        return (seq_t)0;
    }
    cache = tmp;

    /* compute new values using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)(i + 2) * cache[i - 1] - (seq_t)(i - 1) * cache[i - 2];
    }

    cache_size = n + 1;
    return cache[n];
}
