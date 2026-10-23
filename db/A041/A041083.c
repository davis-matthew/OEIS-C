/*
 * A041083  Denominators of continued fraction convergents to sqrt(48).
 * https://oeis.org/A041083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 14*a(n-2) - a(n-4); Initial values: a(0)=1, a(1)=1,
 *            a(2)=13, a(3)=14; Works for all n ≥ 0 with O(n) time and O(n)
 *            space
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values.  'size' is the number of values that have already
       been computed and stored in 'cache'.  'capacity' is the amount of
       memory currently allocated for the array. */
    static seq_t *cache     = NULL;
    static size_t size      = 0;      /* number of computed elements */
    static size_t capacity  = 0;      /* allocated capacity */

    /* Initialise the cache and the four starting terms on the first call. */
    if (size == 0) {
        capacity = 8;                       /* start with some room      */
        cache    = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)14;
        size = 4;                           /* values 0..3 are ready     */
    }

    /* If the requested index is already known, just return it. */
    if (n < size)
        return cache[n];

    /* Grow the buffer if we need to store values up to 'n'. */
    if ((size_t)n + 1 > capacity) {
        /* Double the capacity until it is large enough. */
        while ((size_t)n + 1 > capacity)
            capacity = capacity * 2;
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
    }

    /* Compute the new terms using the 4‑term recurrence. */
    for (idx_t i = size; i <= n; ++i) {
        cache[i] = (seq_t)14 * cache[i - 2] - cache[i - 4];
    }
    size = n + 1;     /* now values 0..n are available */

    return cache[n];
}
