/*
 * A170315  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16 * Σ_{k=1}^{41} a(n−k) − 136 * a(n−42) + b(n), where
 *            b(0)=1, b(42)=1, b(1..41)=2, and b(n)=0 for all other n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of elements stored so far */

    /* remember old capacity before resizing */
    idx_t old_cap = cache_cap;

    /* resize if needed */
    if (cache_cap <= n)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failed – behaviour undefined */
            return (seq_t)0;
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* compute missing terms */
    for (idx_t i = old_cap; i <= n; ++i)
    {
        /* 16 * Σ_{k=1}^{min(41,i)} a(i−k) */
        seq_t sum16 = (seq_t)0;
        idx_t limit = i < 41 ? i : 41;
        for (idx_t k = 1; k <= limit; ++k)
            sum16 += cache[i - k] * (seq_t)16;

        /* b(i) */
        seq_t bi = (seq_t)0;
        if (i == 0 || i == 42)
            bi = (seq_t)1;
        else if (i <= 41)
            bi = (seq_t)2;

        if (i >= 42)
            cache[i] = sum16 - cache[i - 42] * (seq_t)136 + bi;
        else
            cache[i] = sum16 + bi;
    }

    return cache[n];
}
