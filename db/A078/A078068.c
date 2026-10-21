/*
 * A078068  Expansion of (1-x)/(1+2*x+2*x^2-x^3).
 * https://oeis.org/A078068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = -3, a(2) = 4, a(n) = -2*a(n-1) - 2*a(n-2) +
 *            a(n-3) for n > 2
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n < cache_len)
        return cache[n];

    idx_t new_len = n + 1;
    seq_t *tmp = cache == NULL
                ? (seq_t*)malloc(new_len * sizeof(seq_t))
                : (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();

    cache = tmp;

    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
            cache[0] = 1;
        else if (i == 1)
            cache[1] = -3;
        else if (i == 2)
            cache[2] = 4;
        else
            cache[i] = -2 * cache[i-1] - 2 * cache[i-2] + cache[i-3];
    }

    cache_len = n + 1;
    return cache[n];
}
