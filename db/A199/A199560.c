/*
 * A199560  a(n) = (3*9^n + 1)/2.
 * https://oeis.org/A199560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 2; a(n) = 9*a(n-1) - 4 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initial allocation for a(0) */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)2;        /* a(0) = 2 */
    }

    /* grow the cache until it contains the requested term */
    while (n >= cache_size) {
        size_t new_size = cache_size * 2;
        if (new_size <= cache_size) {
            /* protect against overflow when cache_size is large */
            new_size = cache_size + 1;
        }

        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) {
            /* allocation failure – not expected in the test harness */
            return (seq_t)0;
        }
        cache = tmp;

        /* compute new terms using the recurrence */
        for (size_t i = cache_size; i < new_size; ++i) {
            cache[i] = cache[i - 1] * (seq_t)9 - (seq_t)4;
        }
        cache_size = new_size;
    }

    return cache[n];
}
