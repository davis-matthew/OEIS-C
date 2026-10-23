/*
 * A127211  a(n) = 4^n*Lucas(n), where Lucas = A000032.
 * https://oeis.org/A127211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 16*a(n-2), a(0) = 2, a(1) = 4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Initialisation */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)4;
    }

    /* Return if already computed */
    if (n < cap) return cache[n];

    /* Grow the cache to accommodate index n */
    size_t old_cap = cap;
    size_t new_cap = (size_t)n + 1u;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                /* allocation failure */
    cache = tmp;
    cap   = new_cap;

    /* Compute terms up to n using the recurrence */
    for (size_t i = old_cap; i <= (size_t)n; ++i) {
        seq_t term = (cache[i - 1] * (seq_t)4) + (cache[i - 2] * (seq_t)16);
        cache[i] = term;
    }

    return cache[n];
}
