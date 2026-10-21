/*
 * A099729  a(n) = a(n-1)^2 + a(floor(n/2))^2; a(0) = 1.
 * https://oeis.org/A099729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 + a(floor(n/2))^2 ; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed values.  It grows as needed. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;        /* current number of cached entries   */

    size_t needed = (size_t)n + 1;      /* we must have entries 0 … n          */
    size_t old_len = cache_len;         /* length before possible growth       */

    if (needed > cache_len) {
        cache_len = needed;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    }

    /* Compute missing terms.  old_len is the first index not yet computed. */
    for (size_t i = old_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else {
            seq_t prev  = cache[i - 1];
            seq_t half  = cache[i / 2];
            cache[i] = (prev * prev) + (half * half);
        }
    }

    return cache[n];
}
