/*
 * A106853  Expansion of 1/(1 - x + 4*x^2).
 * https://oeis.org/A106853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - 4*a(n-2), a(0) = 1, a(1) = 1
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 30, i64 n <= 63, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first term directly to avoid trivial allocation. */
    if (n == 0)
        return (seq_t)1;

    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;      /* dynamically allocated array of a(n) */
    static idx_t  max_n = 0;         /* index of first element *not yet computed* */
    static size_t capacity = 0;      /* allocated size of cache */

    /* Grow the cache if it is too small. */
    if ((size_t)n + 1 > capacity)
    {
        size_t new_cap = capacity ? capacity * 2 : (size_t)n + 1;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out‑of‑memory fallback */
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise the first two terms if the cache is empty. */
    if (max_n == 0)
    {
        cache[0] = (seq_t)1;
        if (capacity > 1)
            cache[1] = (seq_t)1;
        max_n = 2;   /* up to index 1 inclusive computed */
    }

    /* Compute any missing values up to n. */
    for (idx_t i = max_n; i <= n; ++i)
    {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = prev1 - (prev2 * (seq_t)4);
    }

    max_n = n + 1;   /* next call will start after n */
    return cache[n];
}
