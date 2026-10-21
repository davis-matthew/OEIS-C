/*
 * A077236  a(n) = 4*a(n-1) - a(n-2) with a(0) = 4 and a(1) = 11.
 * https://oeis.org/A077236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(0) = 4, a(1) = 11
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stores computed a(0)…a(k) */
    static idx_t cache_len = 0;     /* current number of stored entries */

    /* If the value is already cached, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* Determine new cache size */
    idx_t new_len = n + 1;
    seq_t *new_cache;

    if (cache == NULL) {
        /* First allocation */
        new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        if (new_cache == NULL)
            return 0;           /* allocation failure: unlikely in tests */
        cache = new_cache;
        cache_len = 0;          /* will be updated after filling */
    } else {
        /* Grow the cache to hold up to a(n) */
        new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (new_cache == NULL)
            return cache[cache_len - 1];  /* fallback on failure */
        cache = new_cache;
        /* cache_len still equals old size; values 0 … cache_len-1 are present */
    }

    /* Compute missing entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[i] = 4;
        else if (i == 1)
            cache[i] = 11;
        else
            cache[i] = 4 * cache[i - 1] - cache[i - 2];
    }

    cache_len = new_len;   /* update stored size */
    return cache[n];
}
