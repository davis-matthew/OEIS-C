/*
 * A099367  a(n) = A054413(n-1)^2, n >= 1.
 * https://oeis.org/A099367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 51*a(n-1) - a(n-2) - 2*(-1)^n Initial values: a(0) = 0,
 *            a(1) = 1 Recurrence is used for n >= 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed values   */
static size_t cache_capacity = 0;    /* allocated size of cache    */
static size_t cache_size = 0;        /* number of valid entries    */

seq_t a(idx_t n) {
    /* Initialise the cache on first call */
    if (cache == NULL) {
        cache_capacity = 4;                      /* initial capacity */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;              /* out of memory: give up */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;                          /* two values stored */
    }

    /* Grow the cache if needed */
    if ((size_t)n >= cache_size) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_capacity) {
            size_t new_capacity = cache_capacity;
            while (new_capacity < needed) new_capacity <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
            if (!tmp) return (seq_t)0;           /* out of memory */
            cache = tmp;
            cache_capacity = new_capacity;
        }

        /* Compute the missing terms up to n */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t val = (seq_t)51 * cache[i - 1] - cache[i - 2];
            if ((i & 1ULL) == 0)          /* i even: subtract 2 */
                val -= (seq_t)2;
            else                          /* i odd : add 2   */
                val += (seq_t)2;
            cache[i] = val;
        }
        cache_size = (size_t)n + 1;
    }
    return cache[n];
}
