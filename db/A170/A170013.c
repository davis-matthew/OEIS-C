/*
 * A170013  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For 1 ≤ n ≤ 35: a(n)=2*∑_{i=1}^{n} a(n-i) + 2. For n≥36
 *            : a(n)=2*∑_{i=1}^{35} a(n-i) – 3*a(n-36)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];

    /* grow the cache to hold terms 0 … n */
    idx_t old_size = cache_size;
    cache_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= 35 && i >= j; ++j)
            sum += cache[i - j];

        seq_t term = (seq_t)2 * sum;
        if (i >= 36)
            term -= (seq_t)3 * cache[i - 36];

        seq_t c = (i == 0) ? 1 : ((i <= 35) ? 2 : 0);
        cache[i] = term + c;
    }

    return cache[n];
}
