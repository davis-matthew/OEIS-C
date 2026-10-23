/*
 * A156605  a(n) = (4^n + 20)/3.
 * https://oeis.org/A156605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n + 20)/3 Equivalent recurrence: a(0) = 7, a(n) =
 *            4*a(n-1) - 20 (n > 0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    size_t idx = (size_t)n;          /* n is small (<= 10000) */

    /* need at least idx+1 elements */
    size_t needed = idx + 1;
    if (needed > cache_len) {
        /* allocate or grow the cache */
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);   /* allocation failure */

        /* if this is the first allocation, initialise a(0) */
        if (cache_len == 0) {
            cache[0] = 7;                /* (4^0 + 20)/3 = 7 */
            cache_len = 1;
        }

        /* compute missing terms using the recurrence */
        seq_t prev = cache[cache_len - 1];
        for (size_t i = cache_len; i < needed; ++i) {
            cache[i] = prev * 4 - 20;    /* a(i) = 4*a(i-1) - 20 */
            prev = cache[i];
        }
        cache_len = needed;
    }

    return cache[idx];
}
