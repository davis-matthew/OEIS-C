/*
 * A142984  a(1) = 1, a(2) = 4, a(n+2) = 4*a(n+1) + (n + 1)*(n + 2)*a(n).
 * https://oeis.org/A142984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence:  a(1)=1, a(2)=4,
   a(n+2)=4*a(n+1)+(n+1)*(n+2)*a(n) */
seq_t a(idx_t n)
{
    /* static cache: cache[i] stores a(i+1) */
    static seq_t *cache = NULL;   /* pointer to cache array */
    static idx_t cache_size = 0;  /* number of elements currently stored */

    if (n == 0)  /* sequence is defined for n >= 1 only */
        return 0;

    /* Extend the cache if necessary */
    if (n > cache_size) {
        /* Allocate or grow the array */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        /* Initialise first elements if they were not yet set */
        if (cache_size == 0) {          /* first allocation */
            cache[0] = 1;               /* a(1) */
            cache_size = 1;
        }
        if (cache_size == 1 && n >= 2) {/* a(2) */
            cache[1] = 4;
            cache_size = 2;
        }

        /* Compute all remaining terms up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* For i >= 3 the recurrence applies */
            if (i >= 3) {
                seq_t term = 4 * cache[i - 2]  /* 4*a(i-1)   */
                           + ((seq_t)(i - 1) * (seq_t)i) * cache[i - 3];
                /* term is a(i); store at index i-1 */
                cache[i - 1] = term;
            }
        }
        cache_size = n;
    }

    /* Return the requested value from the cache */
    return cache[n - 1];
}
