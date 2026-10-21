/*
 * A215560  a(n) = 3*a(n-1) + 46*a(n-2) + a(n-3) with a(0)=a(1)=3, a(2)=101.
 * https://oeis.org/A215560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) + 46*a(n-2) + a(n-3), a(0) = 3, a(1) = 3, a(2) = 101 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to handle any call order */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t last  = 0; /* last computed index */

    /* Ensure cache is initialized with base terms */
    if (cache == NULL) {
        cap = 8; /* initial capacity, grows as needed */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort(); /* abort on allocation failure */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)101;
        last = 2;
    }

    /* If requested n is already computed, just return it */
    if (n <= last) {
        return cache[(size_t)n];
    }

    /* Grow array until it can hold index n */
    while (cap <= (size_t)n) {
        size_t newcap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* Compute additional terms up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        cache[i] = ((seq_t)3 * cache[i - 1]) +
                   ((seq_t)46 * cache[i - 2]) +
                   cache[i - 3];
    }
    last = (size_t)n;
    return cache[(size_t)n];
}
