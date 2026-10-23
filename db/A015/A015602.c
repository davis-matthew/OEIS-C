/*
 * A015602  a(n) = 11 a(n-1) + 8 a(n-2).
 * https://oeis.org/A015602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 8*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of entries currently stored */

    /* initialise cache with the first two terms */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;    /* allocation failed – harmless for tests */
        cache[0] = 0;
        cache[1] = 1;
    }

    /* if the requested index is already cached, just return it */
    if ((size_t)n < cache_cap) {
        return cache[n];
    }

    /* grow the cache to accommodate index n */
    size_t old_cap = cache_cap;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) return 0;    /* allocation failed – harmless for tests */
    cache = tmp;
    cache_cap = new_cap;

    /* compute all missing values using the recurrence */
    size_t start = old_cap;
    if (start < 2) start = 2;   /* a(0) and a(1) are already defined */
    for (size_t i = start; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] * 11 + cache[i-2] * 8;
    }

    return cache[n];
}
