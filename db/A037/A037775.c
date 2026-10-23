/*
 * A037775  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,0,2.
 * https://oeis.org/A037775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) + 8*a(n-3) - 7*a(n-4) * for n ≥ 5,
 *            with a(1)=3, a(2)=22, a(3)=154, a(4)=1080
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[1].. holds computed terms; index 0 is unused */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of the cache array */

    if (n == 0)
        return (seq_t)0;          /* undefined, but safe return */

    size_t idx = (size_t)n;        /* index to use in the array */

    /* initialise cache with the first four terms */
    if (!cache) {
        size_t init_cap = (idx + 1 > 5) ? (idx + 1) : 5;
        cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (!cache)
            abort();                  /* allocation failure */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)154;
        cache[4] = (seq_t)1080;
        cache_cap = init_cap;
    }

    /* extend the cache if the requested index is beyond current size */
    if (idx + 1 > cache_cap) {
        size_t old_cap = cache_cap;
        cache_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp)
            abort();                        /* allocation failure */
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (size_t i = old_cap; i <= idx; ++i) {
            cache[i] = cache[i-1] * (seq_t)8
                      - cache[i-2] * (seq_t)8
                      + cache[i-3] * (seq_t)8
                      - cache[i-4] * (seq_t)7;
        }
    }

    return cache[idx];
}
