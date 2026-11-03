/*
 * A078019  Expansion of (1-x)/(1-x+2*x^2-x^3).
 * https://oeis.org/A078019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 2*a(n-2) + a(n-3); a(0)=1, a(1)=0, a(2)=-2
 * Range:     i8 n <= 17, i16 n <= 37, i32 n <= 77, i64 n <= 155,
 *            i128 n <= 313, big any n
 * Validated: standard, pass, 67 terms through n = 846 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return -2;

    /* Dynamic cache of already computed values */
    static seq_t *cache = NULL;
    static size_t stored = 0;           /* number of values in cache */

    if (cache == NULL) {
        /* Allocate space for the first three terms and initialize them */
        stored = 3;
        cache = mem_new(seq_t, stored);
        if (cache == NULL) return 0;    /* Out of memory – unlikely for this task */
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = -2;
    }

    if ((size_t)n < stored) return cache[(size_t)n];

    /* Expand the cache to include up to n */
    size_t newSize = (size_t)n + 1;
    if (newSize > stored) {
        seq_t *tmp = mem_resize(seq_t, cache, newSize);
        if (tmp == NULL) return 0;        /* Out of memory */
        cache = tmp;
        /* Compute missing terms using the recurrence */
        for (size_t i = stored; i <= n; ++i) {
            cache[i] = cache[i-1] - ((seq_t)2 * cache[i-2]) + cache[i-3];
        }
        stored = newSize;
    }

    return cache[(size_t)n];
}
