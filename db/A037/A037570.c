/*
 * A037570  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,2.
 * https://oeis.org/A037570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) computed from the recurrence of the generating function:
 *            a(1) = 2, a(2) = 11, a(3) = 57, a(4) = 287; for n >= 5: a(n) =
 *            5*a(n-1) + a(n-3) - 5*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; value for n==0 is arbitrary */

    /* static cache for memoisation */
    static seq_t *cache = NULL;        /* base-1 index; cache[0] unused */
    static idx_t cache_cap = 0;        /* number of allocated slots (>=1) */
    static idx_t max_cached = 0;       /* biggest n for which cache[n] is set */

    /* initialise cache lazily */
    if (cache_cap == 0) {
        cache_cap = 8;                       /* initial capacity */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;               /* should never happen */
    }

    /* grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;            /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    if (n > max_cached) {
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i == 1)            cache[1] = 2;
            else if (i == 2)        cache[2] = 11;
            else if (i == 3)        cache[3] = 57;
            else if (i == 4)        cache[4] = 287;
            else                    cache[i] = cache[i-1] * 5 + cache[i-3] - cache[i-4] * 5;
        }
        max_cached = n;
    }

    return cache[n];
}
