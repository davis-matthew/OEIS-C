/*
 * A155017  a(n) = 10*a(n-1) + 90*a(n-2), n>2 ; a(0)=1, a(1)=1, a(2)=19 .
 * https://oeis.org/A155017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=19 ; for n>2, a(n) = 10*a(n-1) + 90*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to memoise computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0U;  /* number of elements stored */

    /* initialise cache if first call */
    if (cache_len == 0U) {
        cache_len = 3U;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) {            /* allocation failure – return 0 as fallback */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)19;
    }

    /* if requested value already computed, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* grow cache to accommodate new index n */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1U) * sizeof(seq_t));
    if (!tmp) {                    /* allocation failure – return 0 as fallback */
        return (seq_t)0;
    }
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)10 * cache[i - 1U] + (seq_t)90 * cache[i - 2U];
    }

    cache_len = n + 1U;
    return cache[n];
}
