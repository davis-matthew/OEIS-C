/*
 * A059158  A hierarchical sequence (S(W'3{2,2}cc) - see A059126).
 * https://oeis.org/A059158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 32*a(n-2), a(0) = 24, a(1) = 432
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t last_computed = 0;

    size_t N = (size_t)n;

    if (cache == NULL)
    {
        size_t init = (N < 2) ? 2 : N + 1;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        cache_len = init;
        cache[0] = (seq_t)24;
        cache[1] = (seq_t)432;
        last_computed = (N >= 1) ? 1 : 0;
    }
    else if (N >= cache_len)
    {
        size_t new_cap = cache_len ? cache_len * 2 : 2;
        if (new_cap <= N) new_cap = N + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_len = new_cap;
    }

    if (N <= last_computed)
        return cache[N];

    for (size_t i = last_computed + 1; i <= N; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)24;
        else if (i == 1)
            cache[i] = (seq_t)432;
        else
            cache[i] = 18 * cache[i - 1] - 32 * cache[i - 2];
    }

    last_computed = N;
    return cache[N];
}
