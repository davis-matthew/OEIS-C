/*
 * A090728  a(n) = 20*a(n-1) - a(n-2), starting with a(0) = 2 and a(1) = 20.
 * https://oeis.org/A090728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 20*a(n-1) - a(n-2); a(0) = 2; a(1) = 20
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of allocated entries */

    /* Ensure base cases are present */
    if (cache_cap < 2) {
        /* Allocate space for indices 0 and 1 */
        cache = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        cache_cap = 2;
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)20;
    }

    if (n < cache_cap)
        return cache[n];

    /* Extend cache to hold up to index n */
    idx_t old_cap = cache_cap;
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    cache_cap = n + 1;

    /* Compute missing terms using the recurrence */
    for (idx_t i = old_cap; i <= n; ++i) {
        cache[i] = (seq_t)20 * cache[i - 1] - cache[i - 2];
    }

    return cache[n];
}
