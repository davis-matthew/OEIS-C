/*
 * A136324  Interleaving of A002450(n), A002450(n) + 1.
 * https://oeis.org/A136324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) - 4*a(n-4) ( a(0)=0, a(1)=1, a(2)=1, a(3)=2 )
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static size_t   len   = 0;

    /* Initial allocation and base values. */
    if (!cache) {
        cap = 16;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache) return 0;               /* Allocation failure – unlikely */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 2;
        len   = 4;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* Allocation failure – abort */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n. */
    if ((size_t)n >= len) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            if (i < 4) continue;          /* Base cases already set */

            cache[i] = (5 * cache[i-2]) - (4 * cache[i-4]);
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
