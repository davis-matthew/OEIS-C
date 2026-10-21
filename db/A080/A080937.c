/*
 * A080937  Number of Catalan paths (nonnegative, starting and ending at 0,
 *          step +/-1) of 2*n steps with all values <= 5.
 * https://oeis.org/A080937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula: a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3),
    with a(0)=1, a(1)=1, a(2)=2.
    Computes the number of Dyck paths of length 2*n staying <=5. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;      /* array of previously computed values */
    static idx_t cache_size = 0;     /* number of terms currently in cache */

    /* initialise the cache with the first three terms */
    if (cache_size == 0) {
        cache_size = 3;                       /* we store indices 0,1,2 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
    }

    /* if we already have the requested term, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* extend the cache up to n */
    idx_t new_size = n + 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)2;
        } else {
            /* recurrence: a(i) = 5*a(i-1) - 6*a(i-2) + a(i-3) */
            cache[i] = (seq_t)(5 * cache[i-1] - 6 * cache[i-2] + cache[i-3]);
        }
    }
    cache_size = new_size;
    return cache[n];
}
