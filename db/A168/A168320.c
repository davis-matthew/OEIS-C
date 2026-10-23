/*
 * A168320  a(-1)=0. a(n)=a(n-1)^2+2^n.
 * https://oeis.org/A168320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 + 2^n, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_n = (idx_t)-1;

    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 1;                    /* a(0) = 1 */
        cached_n = 0;
    }

    if (n <= cached_n) {
        return cache[n];
    }

    /* Grow cache to accommodate up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    cache = new_cache;

    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t sq   = prev * prev;          /* a(i-1)^2 */
        seq_t p2   = pow2(i);              /* 2^i */
        cache[i] = sq + p2;                /* a(i) = sq + 2^i */
    }

    cached_n = n;
    return cache[n];
}
