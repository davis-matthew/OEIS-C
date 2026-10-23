/*
 * A015451  a(n) = 6*a(n-1) + a(n-2) for n > 1, with a(0) = a(1) = 1.
 * https://oeis.org/A015451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + a(n-2) for n > 1; a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to remember already computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated size */
    static size_t last_computed = 0; /* highest index computed */

    /* initialise cache if necessary */
    if (cache == NULL) {
        capacity = 16;                      /* start with some slack */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0;        /* out of memory – shouldn't happen */

        /* base cases */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last_computed = 1;
    }

    /* return immediately if already known */
    if (n <= last_computed) {
        return cache[n];
    }

    /* ensure enough space */
    if ((size_t)n >= capacity) {
        size_t new_capacity = capacity;
        while ((size_t)n >= new_capacity)
            new_capacity *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* out of memory – shouldn't happen */
        cache = tmp;
        capacity = new_capacity;
    }

    /* compute terms until n */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = ( (seq_t)6 * cache[i-1] ) + cache[i-2];
    }
    last_computed = (size_t)n;

    return cache[n];
}
