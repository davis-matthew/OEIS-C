/*
 * A022411  a(n) = a(n-1) + a(n-2) + 1, with a(0)=3, a(1)=12.
 * https://oeis.org/A022411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 1 with a(0) = 3, a(1) = 12
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)12;

    /* Lazily allocate a cache for the sequence values */
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t   cap   = 0;        /* capacity of the array       */
    static idx_t   max_i = 0;        /* furthest index computed     */

    if (!cache) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)12;
        max_i  = 1;
    }

    /* If the requested term is already computed, return it */
    if (n <= max_i) return cache[n];

    /* Grow the array capacity if needed */
    if (cap <= n) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;   /* double until big enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute terms from (max_i + 1) up to n inclusively */
    for (idx_t i = max_i + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)1;
    }
    max_i = n;
    return cache[n];
}
