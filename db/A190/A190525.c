/*
 * A190525  Number of n-step one-sided prudent walks, avoiding exactly two
 *          consecutive west steps (can have three or more west steps).
 * https://oeis.org/A190525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) + a(n-4) with initial values:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=15 (The recurrence is valid for all
 *            n >= 4.) Memoized so that calls in any order remain efficient
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t   len   = 0;  /* number of terms stored (cache[0..len-1]) */

    /* Initialize base terms if this is the first call */
    if (cache == NULL) {
        size_t capacity = (size_t)4;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0;          /* out of memory: failure */
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 6;
        cache[3] = 15;
        len = 4;
    }

    /* If requested term is already known, just return it */
    if (n < len) return cache[n];

    /* Grow cache to include new terms */
    size_t new_capacity = (size_t)(n + 1);
    seq_t *new_cache = (seq_t*)realloc(cache, new_capacity * sizeof(seq_t));
    if (new_cache == NULL) return 0;          /* out of memory */
    cache = new_cache;

    /* Compute successive terms using the recurrence */
    for (idx_t i = len; i <= n; ++i) {
        /* Formula: a(i) = 2*a(i-1) + a(i-2) - a(i-3) + a(i-4) */
        cache[i] = 2*cache[i-1] + cache[i-2] - cache[i-3] + cache[i-4];
    }

    len = n + 1;
    return cache[n];
}
