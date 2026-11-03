/*
 * A097780  Chebyshev polynomials S(n,25) with Diophantine property.
 * https://oeis.org/A097780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1) - a(n-2), a(0) = 1, a(1) = 25
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 658 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t size  = 0;   /* number of computed terms */
    static idx_t cap   = 0;   /* allocated capacity */

    if (n < size)              /* already computed */
        return cache[n];

    /* initialise cache if first call */
    if (size == 0) {
        cap = 2;
        cache = mem_new(seq_t, cap);
        cache[0] = 1;
        cache[1] = 25;
        size = 2;
    }

    /* extend cache up to n */
    while (size <= n) {
        seq_t next = 25 * cache[size - 1] - cache[size - 2];

        if (size >= cap) {            /* grow storage */
            cap *= 2;
            cache = mem_resize(seq_t, cache, cap);
        }

        cache[size] = next;
        ++size;
    }

    return cache[n];
}
