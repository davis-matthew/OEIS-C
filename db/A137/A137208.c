/*
 * A137208  a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3) for n > 2; a(0)=2, a(1)=3,
 *          a(2)=6.
 * https://oeis.org/A137208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3), a(0)=2, a(1)=3, a(2)=6
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* migration of n to size_t for array indexing – n is uint64_t */
    size_t idx = (size_t)n;

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated size */
    static size_t cached = 0;   /* highest index cached (inclusive) */

    /* initialise cache if necessary */
    if (cache == NULL) {
        cap = 4;                /* start with space for indices 0..3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();   /* out of memory – terminate */

        /* initialise known terms */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cached = 2;            /* we have terms up to index 2 stored */
    }

    /* resize cache if needed */
    if (idx > cached) {
        /* double capacity until it covers idx */
        while (cap <= idx) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* out of memory – terminate */
            cache = tmp;
            cap = new_cap;
        }

        /* compute missing terms up to idx */
        for (size_t i = cached + 1; i <= idx; ++i) {
            if (i == 0) { cache[i] = (seq_t)2; continue; }
            if (i == 1) { cache[i] = (seq_t)3; continue; }
            if (i == 2) { cache[i] = (seq_t)6; continue; }

            /* recurrence: a(i)=a(i-1)+4*a(i-2)-4*a(i-3) */
            cache[i] = cache[i-1] + (seq_t)4 * cache[i-2] - (seq_t)4 * cache[i-3];
        }
        cached = idx;
    }

    return cache[idx];
}
