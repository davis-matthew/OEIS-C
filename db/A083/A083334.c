/*
 * A083334  a(n) = 12*a(n-2) - 25*a(n-4) with initial terms 1,6,17,47.
 * https://oeis.org/A083334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12*a(n-2) - 25*a(n-4), a(0) = 1, a(1) = 6, a(2) = 17,
 *            a(3) = 47
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;

    /* if cache is empty, allocate space for the first four terms */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;          /* memory failure, should not happen */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)47;
    }

    /* if the requested term is already known, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* grow the cache to hold terms up to n */
    idx_t new_len = n + 1;          /* +1 because indices start at 0 */
    if (new_len > cache_len) {
        idx_t old_len = cache_len;
        cache_len = new_len;
        seq_t *ptr = (seq_t *)realloc(cache, sizeof(seq_t) * cache_len);
        if (!ptr) return (seq_t)0;   /* memory failure, should not happen */
        cache = ptr;

        /* compute any missing terms using the recurrence */
        for (idx_t i = old_len; i <= n; ++i) {
            /* indices 0..3 are already initialized */
            if (i < 4) continue;
            /* recurrence: 12*a(i-2) - 25*a(i-4) */
            cache[i] = 12 * cache[i - 2] - 25 * cache[i - 4];
        }
    }

    return cache[n];
}
