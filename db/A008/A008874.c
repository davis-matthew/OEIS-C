/*
 * A008874  3x+1 sequence starting at 63.
 * https://oeis.org/A008874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 63; a(n) = 3*a(n-1)+1 if a(n-1) is odd, a(n) = a(n-1)/2
 *            if a(n-1) is even. (Collatz sequence starting at 63)
 * Range:     u8 n <= 2, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache_len == 0) {                /* initialisation */
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)63;
        cache_len = 1;
    }

    if (n < cache_len) return cache[n];

    /* grow cache to hold terms up to n */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        if (prev & (seq_t)1)                /* odd */
            cache[i] = prev * (seq_t)3 + (seq_t)1;
        else                                /* even */
            cache[i] = prev >> 1;
    }

    cache_len = n + 1;
    return cache[n];
}
