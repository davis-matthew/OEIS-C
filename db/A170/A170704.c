/*
 * A170704  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1.
 * For 1≤n≤49:  a(n)=2 + 21·∑_{k=1}^{n} a(n-k)
 * For n=50:    a(50)=1 + 21·∑_{k=1}^{49} a(50-k) – 231·a(0)
 * For n>50:    a(n)=21·∑_{k=1}^{49} a(n-k) – 231·a(n-50)
 *
 * This implements the recurrence derived from the generating
 * function of A170704 and memoises results in a growing static
 * array so that any index can be requested in O(max(n,prev))
 * time. */

#include <stdlib.h>    /* malloc, realloc */
#include <stddef.h>    /* size_t */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t   cache_cap = 0;   /* allocated number of entries   */
    static idx_t    next_computed = 0;  /* first uncomputed index       */

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((cache_cap == 0) ? (size_t)n + 1ULL : cache_cap * 2ULL);
        if (new_cap < (size_t)n + 1ULL)
            new_cap = (size_t)n + 1ULL;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute all terms up to n */
    for (idx_t i = next_computed; i <= n; ++i) {
        seq_t c;
        if (i == 0)
            c = 1;
        else if (i == 50)
            c = 1;
        else if (i < 50)
            c = (seq_t)2;
        else
            c = 0;

        seq_t sum = 0;
        /* sum up to 49 preceding terms (or fewer if i < 49) */
        for (size_t k = 1; k <= 49 && k <= (size_t)i; ++k)
            sum += cache[(size_t)i - k];

        seq_t val = c + (seq_t)21 * sum;
        if (i >= 50)
            val -= (seq_t)231 * cache[(size_t)i - 50];

        cache[(size_t)i] = val;
    }

    next_computed = n + 1;
    return cache[(size_t)n];
}
