/*
 * A239852  Number of n X 2 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or zero plus the sum of elements above
 *          it or one plus the sum of the elements diagonally to its northwest
 *          or one plus the sum of the elements antidiagonally to its
 *          northeast, modulo 4.
 * https://oeis.org/A239852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-5) for n > 9 Initial values: a(1)=2, a(2)=3,
 *            a(3)=5, a(4)=4, a(5)=5, a(6)=5, a(7)=6, a(8)=10, a(9)=11
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 102, u64 n <= 206,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stdint.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset – behaviour below the offset
       does not matter. */
    if (n < 1)
        return 0;

    /* Static memoisation cache. */
    static seq_t *cache           = NULL;
    static idx_t  cache_cap       = 0;   /* allocated size */
    static idx_t  max_computed    = 0;   /* highest n for which cache[n] is valid */

    /* Allocate / grow the cache large enough to hold up to index n. */
    if (cache == NULL) {
        /* Ensure space for the first 10 entries (indices 0..9). */
        cache_cap = (n > 9 ? n + 1 : 10);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;   /* out of memory – unlikely */
        /* Initialise the first 10 entries (index 0 unused). */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;  /* a(1) */
        cache[2] = (seq_t)3;  /* a(2) */
        cache[3] = (seq_t)5;  /* a(3) */
        cache[4] = (seq_t)4;  /* a(4) */
        cache[5] = (seq_t)5;  /* a(5) */
        cache[6] = (seq_t)5;  /* a(6) */
        cache[7] = (seq_t)6;  /* a(7) */
        cache[8] = (seq_t)10; /* a(8) */
        cache[9] = (seq_t)11; /* a(9) */
        max_computed = 9;
    } else if (cache_cap <= n) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;   /* out of memory – unlikely */
        /* The above realloc preserves all previous entries. */
    }

    /* Compute any missing terms up to n. */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* Recurrence: a(i) = a(i-2) + a(i-5) */
            cache[i] = cache[i - 2] + cache[i - 5];
        }
        max_computed = n;
    }

    return cache[n];
}
