/*
 * A166912  a(n) = 20*a(n-1) - 64*a(n-2) - 225 for n > 2; a(0) = 106, a(1) =
 *          8075, a(2) = 114235.
 * https://oeis.org/A166912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=106, a(1)=8075, a(2)=114235; a(n)=20*a(n-1)-64*a(n-2)-225
 *            for n>2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 29,
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

    /* If we already have the value, return it. */
    if (n < cache_size)
        return cache[n];

    /* Lazily initialize the first three terms. */
    if (cache_size == 0) {
        cache_size = 3;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = 106;
        cache[1] = 8075;
        cache[2] = 114235;
    }

    /* Compute any missing terms up to n. */
    if (n >= cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] =
                (seq_t)20 * cache[i - 1]
                - (seq_t)64 * cache[i - 2]
                - (seq_t)225;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
