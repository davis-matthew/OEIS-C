/*
 * A203624  Numbers which are both decagonal and octagonal.
 * https://oeis.org/A203624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 37634*a(n-1)-a(n-2)+16856, a(1)=1, a(2)=54405
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored */

    /* Initialize with the first two terms. */
    if (!cache) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)54405;
    }

    /* Grow cache and compute missing terms up to n. */
    if (cache_size < (size_t)n) {
        size_t new_size = cache_size;
        while (new_size < (size_t)n) new_size <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) abort();
        cache = tmp;

        /* Compute terms from index cache_size to n-1 (0‑based). */
        for (size_t i = cache_size; i < (size_t)n; i++) {
            seq_t prev  = cache[i - 1];     /* a(i)   */
            seq_t prev2 = cache[i - 2];     /* a(i-1) */
            cache[i] = (37634 * prev) - prev2 + (seq_t)16856;
        }
        cache_size = (size_t)n;
    }

    return cache[n - 1];
}
