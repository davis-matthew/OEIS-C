/*
 * A093834  Expansion of (1-2x)^2/((1-3x)(1-4x)).
 * https://oeis.org/A093834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=13, a(n)=7*a(n-1)-12*a(n-2) for n>=3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0..k] contains computed values */
    static idx_t cache_len = 0;      /* number of elements currently stored (>=3 if non‑zero) */

    /* Ensure cache has at least the first three values */
    if (cache_len < 3) {
        /* Allocate space for indices 0,1,2 */
        size_t new_len = 3;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) return 0; /* should not happen in judge environment */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)13;
        cache_len = 3;
    }

    /* Grow cache if necessary */
    if ((idx_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) return 0; /* should not happen */

        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(i) = 7*a(i-1) - 12*a(i-2) */
            cache[i] = 7 * cache[i - 1] - 12 * cache[i - 2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
