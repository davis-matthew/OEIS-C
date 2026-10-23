/*
 * A173566  a(n+1) = a(n)^a(n), with a(1) = 2.
 * https://oeis.org/A173566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2, a(n+1) = a(n)^a(n) */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* cache the sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) {
        /* undefined for n=0; return 0 as a harmless default */
        return 0;
    }

    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = 2;          /* a(1) */
        cache_size = 1;
    }

    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* grow cache to hold up to n terms */
    size_t new_cap = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed - return 0 */
        return 0;
    }
    cache = tmp;

    /* compute new terms iteratively */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* term i uses base cache[i-2] (a(i-1)) */
        cache[i - 1] = big_pow(cache[i - 2], cache[i - 2]);
    }

    cache_size = n;
    return cache[n - 1];
}
