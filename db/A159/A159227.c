/*
 * A159227  1/4 the number of n X n arrays of squares of integers with every
 *          2X2 subblock summing to 27.
 * https://oeis.org/A159227
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A159227
   The sequence satisfies the linear recurrence
   a(n) = a(n-1) + 6*a(n-2) - 6*a(n-3) - 8*a(n-4) + 8*a(n-5)   for n ≥ 7
   with initial values
   a(2)=7, a(3)=22, a(4)=45, a(5)=103, a(6)=193.
   The function is memoised so successive calls, in any order, are
   answered in O(1) after the needed terms have been computed. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* values below the offset are irrelevant */
    if (n < 2) return (seq_t)0;

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* capacity of cache array */

    /* initialise cache with the first needed values (indices 0..6) */
    if (cache_size == 0) {
        cache_size = 7;        /* indices 0..6 are valid after this step */
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure */

        /* unused indices 0 and 1 set to 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)7;      /* a(2) */
        cache[3] = (seq_t)22;     /* a(3) */
        cache[4] = (seq_t)45;     /* a(4) */
        cache[5] = (seq_t)103;    /* a(5) */
        cache[6] = (seq_t)193;    /* a(6) */
    }

    /* expand the cache if it cannot contain n */
    size_t required = (size_t)n + 1;
    if (required > cache_size) {
        size_t new_size = required * 2;          /* overshoot for future calls */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failure */

        /* zero the new portion of the array */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;

        cache     = tmp;
        cache_size = new_size;
    }

    /* compute terms up to n if they are not already present */
    /* start from the first index that is still zero */
    idx_t start = 7;
    for (idx_t i = start; (size_t)i <= n; ++i) {
        /* recurrence uses only indices i-1 .. i-5, all of which are set */
        seq_t term =   cache[i-1];
        term += (seq_t)6 * cache[i-2];
        term -= (seq_t)6 * cache[i-3];
        term -= (seq_t)8 * cache[i-4];
        term += (seq_t)8 * cache[i-5];

        cache[i] = term;
    }

    return cache[n];
}
