/*
 * A174750  x-values in the solution to x^2-38*y^2=1.
 * https://oeis.org/A174750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 74*a(n-1) - a(n-2) (a(1)=1, a(2)=37)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n must be >= 1; behaviour for n < 1 is undefined. */
    if (n < 1) {
        /* Providing a trivial dummy value to avoid undefined behaviour. */
        return (seq_t)0;
    }

    /* Static cache of computed values, indexed from 1. */
    static seq_t *cache = NULL;
    static size_t cap = 0;            /* allocated capacity (number of slots) */
    static idx_t cached_max = 0;      /* highest index that has been computed */

    /* Ensure that cache can hold indices up to n. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < (size_t)n + 1) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – terminate the program. */
            abort();
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise the first two terms if they have not yet been set. */
    if (cached_max < 1) {
        cache[1] = (seq_t)1;
    }
    if (cached_max < 2) {
        cache[2] = (seq_t)37;
    }
    if (cached_max < 2) {
        cached_max = 2;
    }

    /* Compute any missing terms up to n. */
    if (cached_max < n) {
        const seq_t C74 = (seq_t)74;
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            cache[i] = C74 * cache[i - 1] - cache[i - 2];
        }
        cached_max = n;
    }

    return cache[n];
}
