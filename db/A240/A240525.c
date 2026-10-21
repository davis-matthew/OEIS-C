/*
 * A240525  a(n) = 2^(n-2)*(2^(n+4)-(-1)^n+5).
 * https://oeis.org/A240525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) - 16*a(n-3) for n > 2 with initial
 *            values a(0)=5, a(1)=19, a(2)=68
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
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

    /* initialize the cache with the first three terms */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure aborts */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)19;
        cache[2] = (seq_t)68;
        cache_size = 3;
    }

    /* if n is already computed, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* grow the cache to hold term n (and all intermediate terms) */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;               /* allocation failure aborts */
    cache = tmp;

    /* compute new terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term1 = cache[i - 1] * (seq_t)4;
        seq_t term2 = cache[i - 2] * (seq_t)4;
        seq_t term3 = cache[i - 3] * (seq_t)16;
        cache[i] = term1 + term2 - term3;
    }

    cache_size = new_size;
    return cache[n];
}
