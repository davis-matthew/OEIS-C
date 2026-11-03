/*
 * A152448  a(0)=a(1)=1, a(2)=6, a(3)=11; a(n+4) = 10*a(n+2) - a(n).
 * https://oeis.org/A152448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+4) = 10*a(n+2) - a(n); initial values: a(0)=1, a(1)=1,
 *            a(2)=6, a(3)=11
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* if value already computed, just return it */
    if (n < cache_size)
        return cache[n];

    /* enlarge cache to accommodate n */
    idx_t old_size = cache_size;
    cache = mem_resize(seq_t, cache, (n + 1));
    if (cache == NULL)
        abort();                      /* memory allocation failed */

    /* compute new entries in increasing order, using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i == 0)
            cache[0] = 1;
        else if (i == 1)
            cache[1] = 1;
        else if (i == 2)
            cache[2] = 6;
        else if (i == 3)
            cache[3] = 11;
        else
            cache[i] = 10 * cache[i - 2] - cache[i - 4];
    }

    cache_size = n + 1;
    return cache[n];
}
