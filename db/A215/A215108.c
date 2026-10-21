/*
 * A215108  a(n) = A215082(2*n).
 * https://oeis.org/A215108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = a(n-1) + 2*a(n-2) + a(n-3) - 2*a(n-4) - 3*a(n-5) - a(n-6)
   a(0) = 0, a(1) = 1, a(2) = 4, a(3) = 8, a(4) = 17, a(5) = 35
   (OEIS A215108, offset 0)
*/
#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic array of computed terms */
static size_t  cap   = 0;     /* current capacity (number of elements in cache) */

/* initial values a(0)..a(5) */
static const seq_t init[6] = {0, 1, 4, 8, 17, 35};

seq_t a(idx_t n)
{
    /* Return one of the first six known values directly */
    if (n < 6)
        return init[n];

    /* If cache is uninitialised, create it with capacity n+1 */
    if (cap == 0) {
        cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* copy the first six terms */
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        /* compute any remaining terms up to the allocated size */
        for (idx_t i = 6; i < (idx_t)cap; ++i)
            cache[i] = cache[i-1] + 2*cache[i-2] + cache[i-3]
                     - 2*cache[i-4] - 3*cache[i-5] - cache[i-6];
        return cache[n];
    }

    /* If requested index is beyond current capacity, grow the array */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* If cache is ready but not yet filled up to n, compute missing values */
    if (cap >= (size_t)n + 1 && (n >= 6)) {
        /* Find the first index that has not yet been computed */
        idx_t start = 0;
        if (cache[5] == init[5]) start = 6; /* all initial terms are present */
        else {
            /* scan backwards to find last computed term */
            start = 0;
            for (idx_t i = 6; i < (idx_t)cap; ++i) {
                if (cache[i] == (seq_t)0) { start = i; break; }
            }
        }
        /* compute from start up to n */
        for (idx_t i = start; i <= n; ++i)
            cache[i] = cache[i-1] + 2*cache[i-2] + cache[i-3]
                     - 2*cache[i-4] - 3*cache[i-5] - cache[i-6];
    }

    return cache[n];
}
