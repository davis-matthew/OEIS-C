/*
 * A200880  Number of 0..2 arrays x(0..n+1) of n+2 elements without any
 *          interior element greater than both neighbors.
 * https://oeis.org/A200880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 4*a(n-3) - a(n-4) + a(n-5) with
 *            a(1)=22, a(2)=51, a(3)=121, a(4)=292, a(5)=704
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms, indexed from 1. */
    static seq_t *cache = NULL;      /* NULL → no cache allocated yet */
    static idx_t cache_max = 0;      /* highest index computed in cache */

    if (n < 1)
        return 0;                    /* undefined, but safe */

    /* Expand cache if necessary */
    if (n > cache_max) {
        /* new size must hold indices 0..n (0 unused) */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL)
            return 0;                /* allocation failure */
        cache = tmp;

        /* Compute missing terms up to n */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = (seq_t)22;
            else if (i == 2)
                cache[i] = (seq_t)51;
            else if (i == 3)
                cache[i] = (seq_t)121;
            else if (i == 4)
                cache[i] = (seq_t)292;
            else if (i == 5)
                cache[i] = (seq_t)704;
            else
                /* recurrence: a(i) = 3*a(i-1) - 3*a(i-2) + 4*a(i-3)
                                - a(i-4) + a(i-5) */
                cache[i] = 3 * cache[i - 1]
                          - 3 * cache[i - 2]
                          + 4 * cache[i - 3]
                          -   cache[i - 4]
                          +   cache[i - 5];
        }
        cache_max = n;
    }

    return cache[n];
}
