/*
 * A163867  a(n) = a(floor(n/3)) + a(floor(2*n/3)); a(0) = 1.
 * https://oeis.org/A163867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n) = a(floor(n/3)) + a(floor(2*n/3)) */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t capacity   = 0;
    static size_t computed   = 0;   /* number of entries already computed */

    /* Resize storage if needed */
    size_t needed = (size_t)n + 1;
    if (needed > capacity) {
        size_t new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise the base case once */
    if (computed == 0) {
        cache[0] = 1;
        computed = 1;
    }

    /* Compute any missing values up to n */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        cache[i] = cache[i / 3] + cache[(2 * i) / 3];
    }

    /* Keep track of the highest computed index */
    if (computed <= (size_t)n) {
        computed = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
