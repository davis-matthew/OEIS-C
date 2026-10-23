/*
 * A182363  a(n+1) = a(n) + floor(a(n)/10) with a(0) = 10.
 * https://oeis.org/A182363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n) + floor(a(n)/10), a(0) = 10
 * Range:     u8 n <= 38, u16 n <= 97, u32 n <= 213, u64 n <= 446,
 *            u128 n <= 911, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL; /* stored terms */
    static idx_t  stored  = 0;    /* number of terms in cache */

    /* initialize cache with a(0) */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory, unlikely */
        cache[0] = (seq_t)10;
        stored  = 1;
    }

    /* already computed */
    if (n < stored)
        return cache[n];

    /* extend cache up to n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0; /* out of memory, unlikely */
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = stored; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 1] / (seq_t)10;
    }

    stored = n + 1;
    return cache[n];
}
