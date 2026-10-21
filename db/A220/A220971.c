/*
 * A220971  Equals one maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          king-move neighbors in a random 0..1 n X 2 array.
 * https://oeis.org/A220971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A220971: Equals one maps
   Empirical recurrence:
       a(n) = 4*a(n-1) - 4*a(n-2) + 3*a(n-3) - 5*a(n-4)
              - 3*a(n-5) + 6*a(n-6) - 8*a(n-7)   for n > 10
   Initial values (offset 1):
       a(1) = 2,  a(2) = 2,  a(3) = 9,  a(4) = 23,  a(5) = 58,
       a(6) = 149, a(7) = 379, a(8) = 969, a(9) = 2472, a(10) = 6304
   The function memoises results to allow calls in arbitrary order.
   It uses only integer arithmetic compatible with all the supported
   seq_t sizes.                                               */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* static cache stores a(1)..a(k) at cache[k-1] */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* initialise cache once with the first 10 values */
    if (cache == NULL) {
        cache_cap = 10;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)23;
        cache[4] = (seq_t)58;
        cache[5] = (seq_t)149;
        cache[6] = (seq_t)379;
        cache[7] = (seq_t)969;
        cache[8] = (seq_t)2472;
        cache[9] = (seq_t)6304;
        cache_len = 10;
    }

    /* return early if the requested term is already calculated */
    if ((idx_t)n <= cache_len)
        return cache[n - 1];

    /* grow the cache if necessary */
    if ((idx_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();            /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t k = (idx_t)cache_len + 1; k <= n; ++k) {
        /* indices: cache[k-2] = a(k-1), ..., cache[k-8] = a(k-7) */
        seq_t val =
            4 * cache[k - 2]          /* 4*a(k-1) */
          - 4 * cache[k - 3]          /* -4*a(k-2) */
          + 3 * cache[k - 4]          /* +3*a(k-3) */
          - 5 * cache[k - 5]          /* -5*a(k-4) */
          - 3 * cache[k - 6]          /* -3*a(k-5) */
          + 6 * cache[k - 7]          /* +6*a(k-6) */
          - 8 * cache[k - 8];         /* -8*a(k-7) */
        cache[k - 1] = val;
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
