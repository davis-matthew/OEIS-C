/*
 * A133369  a(n+1) = (3*a(n) + 2*a(n-1)) mod 37; a(0) = 0, a(1) = 1.
 * https://oeis.org/A133369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1400, u16 n <= 1400, u32 n <= 1400, u64 n <= 1400,
 *            u128 n <= 1400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+1) = (3*a(n) + 2*a(n-1)) mod 37; a(0)=0, a(1)=1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with first two terms if empty */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* extend cache if needed */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();            /* allocation failed */
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (( (seq_t)3 * cache[i - 1] ) +
                        ( (seq_t)2 * cache[i - 2] )) % (seq_t)37;
        }
        cache_len = new_len;
    }

    return cache[n];
}
