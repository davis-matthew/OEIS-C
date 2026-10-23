/*
 * A020543  a(0) = 1, a(1) = 1, a(n+1) = (n+1)*a(n) + n.
 * https://oeis.org/A020543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*n + (n-1), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached values 0..cache_last */
    static idx_t cache_last = (idx_t)-1;

    /* Initial allocation for indices 0 and 1.  Uses explicit casts
       so the code is valid C++ as well. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_last = 1;
    }

    /* Extend the cache up to n if necessary. */
    if (n > cache_last) {
        size_t new_size = ((size_t)n + 1) * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, new_size);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)i + (seq_t)(i - 1);
        }
        cache_last = n;
    }

    return cache[n];
}
