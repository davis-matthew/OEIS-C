/*
 * A038725  a(n) = 6*a(n-1) - a(n-2), n >= 2, a(0)=1, a(1)=2.
 * https://oeis.org/A038725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 2, a(n) = 6 * a(n-1) - a(n-2) for n >= 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0..size-1] holds a(0..size-1) */
    static idx_t size  = 0;       /* number of cached entries */

    if (n + 1 > size) {           /* need to grow the cache */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();  /* allocation failure */
        cache = new_cache;

        /* initialise base values if not already set */
        if (size < 1) cache[0] = 1;
        if (size < 2 && new_size >= 2) cache[1] = 2;

        /* compute new entries using the recurrence */
        idx_t start = (size < 2 ? 2 : size);
        for (idx_t i = start; i <= n; ++i)
            cache[i] = 6 * cache[i-1] - cache[i-2];

        size = new_size;
    }

    return cache[n];
}
