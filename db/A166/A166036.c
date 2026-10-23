/*
 * A166036  a(n) = (4^n+8*(-5)^n)/9.
 * https://oeis.org/A166036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1) + 20*a(n-2), a(0) = 1, a(1) = -4
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL; /* cache of previously computed terms */
    static size_t cache_len = 0; /* number of terms stored in cache */

    /* If the requested term is already cached, return it immediately */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* Need to extend the cache up to index n (inclusive) */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure – abort, as this is unexpected in the test harness */
        abort();
    }
    cache = tmp;

    /* Compute and store new terms using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else if (i == 1) {
            cache[i] = -4;
        } else {
            cache[i] = -cache[i - 1] + (seq_t)20 * cache[i - 2];
        }
    }

    cache_len = new_len;
    return cache[(size_t)n];
}
