/*
 * A042298  Numerators of continued fraction convergents to sqrt(675).
 * https://oeis.org/A042298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 52*a(n-2) - a(n-4); a(0)=25, a(1)=26, a(2)=1325,
 *            a(3)=1351
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialize cache with first four terms */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) return (seq_t)0;   /* safety: memory allocation failed */
        cache[0] = (seq_t)25;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)1325;
        cache[3] = (seq_t)1351;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* extend the cache to size n+1 */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (tmp == NULL) return (seq_t)0;   /* safety: memory allocation failed */
    cache = tmp;

    /* compute missing terms using the recurrence relation */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)(52 * cache[i - 2] - cache[i - 4]);
    }

    cache_len = new_len;
    return cache[n];
}
