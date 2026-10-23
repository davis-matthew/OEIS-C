/*
 * A183813  1/81 the number of (n+1) X 3 0..2 arrays with no 2 X 2 subblock
 *          being a reflection across the shared element pair of any
 *          horizontal or vertical neighbor.
 * https://oeis.org/A183813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n‑1) + 120*a(n‑2) – 330*a(n‑3); a(1) = 8, a(2) =
 *            162, a(3) = 3256
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below computes the required term using an
   efficient O(n) recurrence with memoisation.  It works for any
   integer type assigned to seq_t and idx_t by the build system. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Cache for memoisation: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initialise the first three values */
    if (!cache) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)8;                  /* a(1) */
        cache[1] = (seq_t)162;                /* a(2) */
        cache[2] = (seq_t)3256;               /* a(3) */
        cache_size = 3;
    }

    /* If the value is already cached, return it */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* Increase cache size to accommodate a(n) */
    size_t old_size = cache_size;
    size_t new_size = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    /* Handle allocation failure by allocating a fresh block */
    if (!tmp) {
        tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        for (size_t i = 0; i < old_size; ++i)
            tmp[i] = cache[i];
        free(cache);
    }

    cache = tmp;
    cache_size = new_size;

    /* Compute the new values using the recurrence */
    for (size_t i = old_size; i < new_size; ++i) {
        cache[i] = (seq_t)15 * cache[i - 1]
                 + (seq_t)120 * cache[i - 2]
                 - (seq_t)330 * cache[i - 3];
    }

    return cache[n - 1];
}
