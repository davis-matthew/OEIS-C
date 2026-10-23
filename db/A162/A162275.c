/*
 * A162275  a(n) = 10*a(n-1) - 22*a(n-2) for n > 1; a(0) = 2, a(1) = 13.
 * https://oeis.org/A162275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 22*a(n-2), a(0) = 2, a(1) = 13
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* initialise cache with first two terms */
    if (cache == NULL)
    {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) abort();           /* allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)13;
        cap = 2;
    }

    /* if n already computed, return it */
    if (n < cap) return cache[n];

    /* extend cache up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL) abort();           /* allocation failure */
    cache = new_cache;

    /* compute missing terms */
    for (size_t i = cap; i <= (size_t)n; ++i)
    {
        /* recurrence: a(i) = 10*a(i-1) - 22*a(i-2) */
        cache[i] = (seq_t)10 * cache[i - 1] - (seq_t)22 * cache[i - 2];
    }

    cap = n + 1;
    return cache[n];
}
