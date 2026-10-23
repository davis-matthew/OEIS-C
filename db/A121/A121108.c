/*
 * A121108  Trajectory of 2677889 under "x -> product of digits of x" map.
 * https://oeis.org/A121108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n+1) = product of digits of a(n), with a(1) = 2677889
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                   /* undefined for n < 1 */

    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_up_to = 0;  /* largest n that has been computed */
    static idx_t capacity   = 0;    /* allocated size of cache */

    const seq_t start_value = (seq_t)2677889; /* initial term a(1) */

    /* Initialise cache on first call */
    if (cache == NULL) {
        capacity = 8;                     /* small initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * (capacity + 1)); /* index 0 unused */
        cache[1] = start_value;
        cached_up_to = 1;
    }

    /* Grow cache if requested index is beyond current capacity */
    if (n > capacity) {
        idx_t new_capacity = n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_capacity + 1));
        if (tmp == NULL)
            return 0;                    /* allocation failed */
        cache = tmp;
        capacity = new_capacity;
    }

    /* Compute any missing terms up to n */
    while (cached_up_to < n) {
        seq_t val = cache[cached_up_to];
        seq_t prod = 1;

        /* product of digits of val */
        if (val == 0)
            prod = 0;
        else {
            seq_t t = val;
            while (t > 0) {
                prod *= (t % 10);
                t /= 10;
            }
        }

        ++cached_up_to;            /* now cached_up_to points to the next term */
        cache[cached_up_to] = prod;
    }

    return cache[n];
}
