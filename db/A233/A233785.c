/*
 * A233785  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the squares of all six edge and diagonal
 *          differences equal to 11.
 * https://oeis.org/A233785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-2) - 40*a(n-4) (for n >= 5) Initial values:
 *            a(1)=48, a(2)=188, a(3)=720, a(4)=2856 Note: a(0) is undefined;
 *            the function returns 0 for n <= 0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed terms */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;  /* last index stored (>=0) */

    if (n <= 0)
        return 0;   /* undefined for non‑positive indices */

    size_t idx = (size_t)n;   /* n is guaranteed to be small for this sequence */

    /* initialise cache on first call */
    if (cached_len == 0) {
        cache = (seq_t *)calloc(5, sizeof(seq_t));   /* indices 0..4 */
        if (!cache)
            return 0;   /* allocation failure – unlikely for allowed ranges */
        cache[1] = (seq_t)48;
        cache[2] = (seq_t)188;
        cache[3] = (seq_t)720;
        cache[4] = (seq_t)2856;
        cached_len = 4;
    }

    /* grow cache and compute missing terms up to idx */
    if (idx > cached_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, (idx + 1) * sizeof(seq_t));
        if (!new_cache)
            return 0;   /* allocation failure – unlikely */
        cache = new_cache;

        /* compute using the recurrence */
        for (size_t i = cached_len + 1; i <= idx; ++i) {
            /* recurrence holds for i >= 5; indices 1..4 are already set */
            cache[i] = (seq_t)18 * cache[i - 2] - (seq_t)40 * cache[i - 4];
        }
        cached_len = idx;
    }

    return cache[idx];
}
