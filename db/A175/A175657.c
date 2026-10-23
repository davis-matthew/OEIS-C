/*
 * A175657  Eight bishops and one elephant on a 3 X 3 chessboard: a(n) = 3*2^n
 *          - 2*F(n+1), with F(n) = A000045(n).
 * https://oeis.org/A175657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) – a(n-2) – 2*a(n-3), a(0)=1, a(1)=4, a(2)=8
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoizes all values it computes, so that calls in
   arbitrary order are fast. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for all i <= cache_last. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* actual allocated size */
    static size_t cache_last = (size_t)-1;/* initialized flag */

    size_t target = (size_t)n;

    /* Initialise the cache on the first call. */
    if (cache_last == (size_t)-1) {
        cache_cap = 4;   /* capacity to hold indices 0..3 (initial values 0..2) */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache)
            return (seq_t)0;               /* OOM: just return 0 */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)8;
        cache_last = 2;
    }

    /* If the requested value is already known, return it immediately. */
    if (target <= cache_last)
        return cache[target];

    /* Ensure there is enough space for all indices up to target. */
    if (target + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= target)
            new_cap <<= 1;               /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                        /* OOM: keep old cache, return 0 */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to target using the recurrence. */
    for (size_t i = cache_last + 1; i <= target; ++i) {
        seq_t val = ((seq_t)3 * cache[i - 1]) - cache[i - 2] -
                    ((seq_t)2 * cache[i - 3]);
        cache[i] = val;
    }
    cache_last = target;
    return cache[target];
}
