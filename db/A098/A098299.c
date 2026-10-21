/*
 * A098299  Member r=14 of the family of Chebyshev sequences S_r(n) defined in
 *          A092184.
 * https://oeis.org/A098299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - a(n-2) + 2, a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    /* Ensure the cache is initialized with the first two terms. */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n. */
    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(i) = 12*a(i-1) - a(i-2) + 2 */
            cache[i] = 12 * cache[i-1] - cache[i-2] + 2;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
