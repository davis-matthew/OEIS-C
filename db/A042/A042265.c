/*
 * A042265  Denominators of continued fraction convergents to sqrt(658).
 * https://oeis.org/A042265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 3386*a(n-8) - a(n-16) for n>15 * Initial values (a(0) ...
 *            a(15)): *
 *            1,1,2,3,20,23,43,66,3343,3409,6752,10161,67718,77879,145597,223476
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms – used for n < 16 and to seed the cache */
    static const seq_t init_vals[16] = {
        1, 1, 2, 3, 20, 23, 43, 66,
        3343, 3409, 6752, 10161, 67718, 77879, 145597, 223476
    };

    /* Cached values for n >= 16.  The array grows as needed.
       The cache holds values 0 ... last_computed inclusive. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;            /* allocated capacity of cache */
    static size_t last_computed = (size_t)-1; /* largest index that has been computed */

    size_t target = (size_t)n;               /* safe C index */

    /* For the first 16 entries we can return the constants directly. */
    if (target < 16)
        return init_vals[target];

    /* Allocate or enlarge the cache so that index 'target' exists. */
    if (cache_size <= target) {
        size_t new_size = target + 1;
        if (cache == NULL) {
            /* First allocation: zero‑initialize the whole array */
            cache = (seq_t*)calloc(new_size, sizeof(seq_t));
            if (!cache)
                return (seq_t)0;           /* allocation failed – return 0 */
            cache_size = new_size;

            /* Set the initial 16 terms */
            for (size_t i = 0; i < 16; ++i)
                cache[i] = init_vals[i];

            last_computed = 15;
        } else {
            /* Resize the array, leaving previous values intact */
            cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
            if (!cache)
                return (seq_t)0;
            cache_size = new_size;
            /* The newly added entries will be filled by the recurrence. */
        }
    }

    /* If the cache was just created, initialise the first 16 terms. */
    if (last_computed == (size_t)-1) {
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
        last_computed = 15;
    }

    /* Compute any missing terms up to 'target'. */
    size_t start = last_computed + 1;
    if (start < 16)
        start = 16;                     /* recurrence is valid only for n>=16 */

    for (size_t j = start; j <= target; ++j) {
        cache[j] = (seq_t)3386 * cache[j - 8] - cache[j - 16];
    }

    last_computed = target;
    return cache[target];
}
