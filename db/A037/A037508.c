/*
 * A037508  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,0.
 * https://oeis.org/A037508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(7^(n+1)/38) Recurrence implementation: a(0) = 0,
 *            a(1) = 1, a(2) = 9, a(3) = 63 a(n) = 7*a(n-1) + a(n-3) -
 *            7*a(n-4) (n >= 4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_last = 0; /* index of last computed term */

    /* initialise cache for indices 0..3 */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;          /* a(0) */
        cache[1] = 1;          /* a(1) */
        cache[2] = 9;          /* a(2) */
        cache[3] = 63;         /* a(3) */
        cache_last = 3;
    }

    /* handle requested n < 0 (should not happen, but safe) */
    if (n == 0) return cache[0];

    /* extend cache up to n */
    while (cache_last < n) {
        /* compute a(cache_last + 1) using recurrence */
        seq_t val;
        size_t next = cache_last + 1;
        if (next == 1) {
            val = 1;
        } else if (next == 2) {
            val = 9;
        } else if (next == 3) {
            val = 63;
        } else {
            /* a(n) = 7*a(n-1) + a(n-3) - 7*a(n-4) */
            val = (7 * cache[cache_last]) + cache[cache_last - 2] - (7 * cache[cache_last - 3]);
        }

        /* grow cache if needed */
        if (next >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) {
                /* allocation failed; fallback to recomputing recursively */
                return val;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[++cache_last] = val;
    }

    return cache[n];
}
