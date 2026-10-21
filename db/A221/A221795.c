/*
 * A221795  Number of n X 2 arrays of occupancy after each element moves to
 *          some horizontal or vertical neighbor, without move-in move-out
 *          straight through or left turns.
 * https://oeis.org/A221795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + a(n-3) - 4*a(n-4) with initial
 *            values: a(1)=1, a(2)=5, a(3)=12, a(4)=39
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(1) has offset 1; behaviour for n<1 is irrelevant. */
    if (n == 0) return 0;

    /* static cache of computed terms */
    static seq_t *cache = NULL;  /* array of a(i) with a(1) at index 0 */
    static size_t cache_cap = 0; /* allocated size */
    static size_t cache_len = 0; /* number of terms currently stored */

    /* convert idx_t to size_t (safe for n up to 10 000 in practice) */
    size_t needed = (size_t)n;

    /* grow the cache if necessary */
    if (needed > cache_cap) {
        seq_t *new_cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) return 0;            /* allocation failed – return 0 */
        cache = new_cache;
        cache_cap = needed;
    }

    /* compute new terms up to n */
    for (size_t i = cache_len; i < needed; ++i) {
        seq_t val;
        switch (i) {
            case 0: val = (seq_t)1;   break;            /* a(1) */
            case 1: val = (seq_t)5;   break;            /* a(2) */
            case 2: val = (seq_t)12;  break;            /* a(3) */
            case 3: val = (seq_t)39;  break;            /* a(4) */
            default:
                /* recurrence: a(i+1) = a(i) + 6*a(i-1) + a(i-2) - 4*a(i-3) */
                val = cache[i-1] + (seq_t)6 * cache[i-2]
                    + cache[i-3] - (seq_t)4 * cache[i-4];
                break;
        }
        cache[i] = val;
    }
    cache_len = needed;

    return cache[needed - 1];
}
