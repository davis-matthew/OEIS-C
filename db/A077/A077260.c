/*
 * A077260  Triangular numbers that are 1/5 of a triangular number.
 * https://oeis.org/A077260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 322*a(n-2) - a(n-4) + 24 for n>=4 Initial values: a(0)=0,
 *            a(1)=3, a(2)=21, a(3)=990
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS A077260.
   Uses a recurrence and memoises results to allow arbitrary order of calls. */
seq_t a(idx_t n)
{
    static idx_t cached_max = 3;     /* highest index already computed */
    static idx_t cache_cap = 4;      /* size of allocated cache array   */
    static seq_t *cache = NULL;      /* memoisation array               */

    /* Initialise cache with the first four terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0; /* fallback on allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)990;
    }

    if (n <= cached_max)  /* already computed */
        return cache[n];

    /* Grow cache if necessary */
    if ((idx_t)cache_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *new_mem = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_mem) return (seq_t)0; /* fallback on allocation failure */
        cache = new_mem;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i < 4) { /* base terms already stored */
            /* nothing to do */
        } else {
            seq_t temp = cache[i - 2];
            temp = temp * (seq_t)322;
            temp += (seq_t)24;
            temp -= cache[i - 4];
            cache[i] = temp;
        }
    }

    cached_max = n;
    return cache[n];
}
