/*
 * A053088  a(n) = 3*a(n-2) + 2*a(n-3) for n > 2, a(0)=1, a(1)=0, a(2)=3.
 * https://oeis.org/A053088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-3), a(0)=1, a(1)=0, a(2)=3
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Return if already computed */
    if (n < cache_size)
        return cache[n];

    /* Need to compute up to n (inclusive) */
    idx_t new_size = n + 1;

    /* Reallocate cache */
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL)
        abort();                    /* allocation failed */
    cache = tmp;

    /* Initialise base terms that are not yet set */
    if (cache_size == 0) {
        if (new_size > 0) cache[0] = (seq_t)1;
        if (new_size > 1) cache[1] = (seq_t)0;
        if (new_size > 2) cache[2] = (seq_t)3;
    } else {
        if (cache_size <= 1 && new_size > 1) cache[1] = (seq_t)0;
        if (cache_size <= 2 && new_size > 2) cache[2] = (seq_t)3;
    }

    /* Compute new terms using recurrence */
    idx_t start = (cache_size > 3u) ? cache_size : (idx_t)3;
    for (idx_t i = start; i < new_size; ++i) {
        /* For i>=3, i-2 and i-3 are valid positive indices */
        cache[i] = (seq_t)3 * cache[i - 2] + (seq_t)2 * cache[i - 3];
    }

    cache_size = new_size;
    return cache[n];
}
