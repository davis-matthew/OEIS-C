/*
 * A162919  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=50, a(2)=2450, a(3)=118825,
   a(n)=48*a(n-1)+48*a(n-2)-1176*a(n-3)  for n≥4. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t  last   = (idx_t)-1;

    /* first time: allocate 4 entries */
    if (!cache) {
        cap = 4;
#ifdef __cplusplus
        cache = new seq_t[cap];          /* C++ (default-construct) */
        if (!cache) return (seq_t)0;
#else
        cache = malloc(cap * sizeof(seq_t)); /* C (malloc) */
        if (!cache) return (seq_t)0;
#endif
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)50;
        cache[2] = (seq_t)2450;
        cache[3] = (seq_t)118825;
        last = 3;
    }

    /* requested value already in cache */
    if ((size_t)n <= last)
        return cache[(size_t)n];

    /* enlarge cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cap * 2)
            new_cap = cap * 2;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        if (!tmp) return (seq_t)0;
        for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
#endif
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = (seq_t)48 * cache[(size_t)(i - 1)]
                     + (seq_t)48 * cache[(size_t)(i - 2)];
        term -= (seq_t)1176 * cache[(size_t)(i - 3)];
        cache[(size_t)i] = term;
    }
    last = n;
    return cache[(size_t)n];
}
