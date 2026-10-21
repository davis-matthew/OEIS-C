/*
 * A233231  a(n) = 10*a(n-3) - a(n-6) + 4 for n>5, a(0)=2, a(1)=3, a(2)=5,
 *          a(3)=12, a(4)=29, a(5)=51.
 * https://oeis.org/A233231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-3) - a(n-6) + 4 (n>5),
 *            a(0)=2,a(1)=3,a(2)=5,a(3)=12,a(4)=29,a(5)=51
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values of the sequence */
    static const seq_t init[6] = {2, 3, 5, 12, 29, 51};

    /* Dynamic cache for computed terms */
    static seq_t *cache   = NULL;   /* pointer to allocated array */
    static size_t   cap    = 0;     /* allocated capacity (in terms) */
    static size_t   size   = 0;     /* number of terms already computed */

    /* Return one of the known terms directly */
    if (n < 6)
        return init[(size_t)n];

    /* Ensure the cache has enough space to store the requested index */
    if (cap < (size_t)n + 1) {
        size_t new_cap = cap ? cap * 2 : 8;
        if (new_cap < (size_t)n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) /* allocation failed – in a real system handle error */
            exit(EXIT_FAILURE);
        cache = tmp;
        cap   = new_cap;
    }

    /* Lazily compute and store terms up to n */
    if (size <= (size_t)n) {
        /* Initialise cache with the first six values if it is empty */
        if (size == 0) {
            for (size_t i = 0; i < 6; ++i)
                cache[i] = init[i];
            size = 6;
        }

        /* Compute remaining terms using the recurrence */
        for (size_t i = size; i <= (size_t)n; ++i) {
            cache[i] = cache[i-3] * (seq_t)10 - cache[i-6] + (seq_t)4;
        }
        size = n + 1;
    }

    return cache[(size_t)n];
}
