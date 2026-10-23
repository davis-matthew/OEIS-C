/*
 * A037513  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,1.
 * https://oeis.org/A037513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(11*4^n/21) Recurrence: a(n) = 4*a(n-1) + a(n-3) -
 *            4*a(n-4), with a(1)=2, a(2)=8, a(3)=33, a(4)=134
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; values below are unused */

    /* Static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* highest index that has been computed */

    /* Allocate or expand cache if necessary */
    if (!cache) {
        size_t alloc = (n > 4) ? (size_t)n + 1 : 5;   /* ensure space for indices 1..4 */
        cache = (seq_t*)malloc(sizeof(seq_t) * alloc);
        if (!cache) return 0;                       /* allocation failure */

        /* initialise the first four terms (always present) */
        cache[1] = 2;
        cache[2] = 8;
        cache[3] = 33;
        cache[4] = 134;
        cache_size = 4;
    }

    if (n > cache_size) {
        /* Expand the array to hold up to n */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * ((size_t)n + 1));
        if (!tmp) return 0;                       /* allocation failure */
        cache = tmp;

        /* Compute any missing terms using the recurrence */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 4) continue;                 /* already initialised */
            cache[i] = cache[i - 1] * 4
                     + cache[i - 3]
                     - cache[i - 4] * 4;
        }
        cache_size = n;
    }

    return cache[n];
}
