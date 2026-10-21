/*
 * A208737  Number of nonisomorphic graded posets with 0 and 1 and non-uniform
 *          Hasse graph of rank n, with no 3-element antichain.
 * https://oeis.org/A208737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 36*a(n-2) + 57*a(n-3) - 39*a(n-4) + 9*a(n-5)
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=7, a(5)=37
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of allocated slots */
    static idx_t max_comp   = 0;      /* highest index already computed    */

    /* Initial terms of the sequence (indices 0..5) */
    static const seq_t init[6] = { (seq_t)0,
                                   (seq_t)0,
                                   (seq_t)0,
                                   (seq_t)1,
                                   (seq_t)7,
                                   (seq_t)37 };

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();      /* out of memory */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init[i];
        max_comp = 5;
    }

    /* Grow the cache if needed to hold up to index n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();        /* out of memory */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;                 /* initialize new slots */
        cache_size = new_size;
    }

    /* Compute any missing terms up to n */
    if (n > max_comp) {
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            /* Compute the recurrence in a way that avoids
               unsigned underflow: part1 ≥ part2 for all n.     */
            seq_t part1 = cache[i-1] * (seq_t)10
                        + cache[i-3] * (seq_t)57
                        + cache[i-5] * (seq_t) 9;
            seq_t part2 = cache[i-2] * (seq_t)36
                        + cache[i-4] * (seq_t)39;
            cache[i] = part1 - part2;
        }
        max_comp = n;
    }

    return cache[n];
}
