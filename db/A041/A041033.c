/*
 * A041033  Denominators of continued fraction convergents to sqrt(21).
 * https://oeis.org/A041033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 110*a(n-6) - a(n-12) for n >= 12 initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=5, a(4)=7, a(5)=12, a(6)=103, a(7)=115,
 *            a(8)=218, a(9)=551, a(10)=769, a(11)=1320
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t capacity  = 0;      /* current allocated size of cache   */
    static idx_t max_computed = (idx_t)-1;   /* highest index that has been filled */

    /* initial 12 terms */
    static const seq_t init[12] = {
        (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)5,
        (seq_t)7,  (seq_t)12, (seq_t)103, (seq_t)115,
        (seq_t)218, (seq_t)551, (seq_t)769, (seq_t)1320
    };

    /* first call: allocate cache and copy the initial values */
    if (capacity == 0) {
        capacity = 12;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed – best effort */
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        max_computed = 11;
    }

    /* if the requested index is already in the cache, just return it */
    if (n < capacity)
        return cache[n];

    /* otherwise grow the cache to at least n+1 and compute missing terms */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;  /* allocation failed – best effort */
    cache = tmp;
    capacity = new_cap;

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i < 12)
            cache[i] = init[i];
        else
            cache[i] = ((seq_t)110 * cache[i - 6]) - cache[i - 12];
    }

    max_computed = n;
    return cache[n];
}
