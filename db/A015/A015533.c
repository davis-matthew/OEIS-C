/*
 * A015533  a(n) = 4*a(n-1) + 9*a(n-2).
 * https://oeis.org/A015533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 9*a(n-2); a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* handle the base cases directly */
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    /* first time: allocate array for indices 0 and 1 */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)
            abort();
        cache[0] = 0;
        cache[1] = 1;
    }

    /* ensure the cache is large enough for index n */
    size_t nn = (size_t)n;
    if (nn >= cache_size) {
        size_t new_size = nn + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;

        /* compute all new entries up to n inclusive */
        for (size_t i = cache_size; i <= nn; ++i) {
            cache[i] = 4 * cache[i - 1] + 9 * cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[nn];
}
