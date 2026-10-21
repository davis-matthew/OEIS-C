/*
 * A205961  Expansion of 1/(-32*x^5 + 8*x^3 - 4*x^2 - x + 1).
 * https://oeis.org/A205961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 8*a(n-3) + 32*a(n-5) Initial values:
 *            a(0)=1, a(1)=1, a(2)=5, a(3)=1, a(4)=13
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t sz = 0;            /* number of terms currently cached */

    /* Initialise cache with the first five terms */
    if (sz == 0) {
        sz = 5;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)13;
    }

    /* Grow cache if the requested index is beyond the current size */
    if (n + 1 > sz) {
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Compute new terms using the recurrence */
        for (idx_t i = sz; i < new_sz; ++i) {
            if (i < 5) continue;                 /* already initialised */
            seq_t temp = cache[i-1]
                        + (seq_t)4 * cache[i-2]
                        + (seq_t)32 * cache[i-5];
            cache[i] = temp - (seq_t)8 * cache[i-3];
        }
        sz = new_sz;
    }

    return cache[n];
}
