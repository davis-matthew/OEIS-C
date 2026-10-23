/*
 * A164870  The number of permutations of length n that can be sorted by 2 pop
 *          stacks in parallel.
 * https://oeis.org/A164870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 6*a(n-3), with initial values
 *            a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, but safe */

    /* cache previously computed terms to allow arbitrary order of calls */
    static seq_t *cache   = NULL;   /* array storing a(1), a(2), ... */
    static idx_t cached_n = 0;     /* number of terms currently cached */

    /* initialise cache with first three terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)3);
        if (!cache)
            return (seq_t)0; /* allocation failure – unlikely */
        cache[0] = (seq_t)1;  /* a(1) */
        cache[1] = (seq_t)2;  /* a(2) */
        cache[2] = (seq_t)6;  /* a(3) */
        cached_n = 3;
    }

    /* if the requested term is already in cache, return it */
    if (n <= cached_n)
        return cache[n - 1];

    /* extend cache to size n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
    if (!new_cache)
        return (seq_t)0; /* reallocation failure – unlikely */
    cache = new_cache;

    /* compute missing terms using the recurrence */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* recurrence: a(i) = 6*a(i-1) - 10*a(i-2) + 6*a(i-3) */
        seq_t v = (seq_t)6 * cache[i - 2]   /* 6*a(i-1) */
                - (seq_t)10 * cache[i - 3]  /* 10*a(i-2) */
                + (seq_t)6 * cache[i - 4]; /* 6*a(i-3) */
        cache[i - 1] = v;
    }

    cached_n = n;
    return cache[n - 1];
}
