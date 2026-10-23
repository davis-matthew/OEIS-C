/*
 * A022397  Fibonacci sequence beginning 1, 27.
 * https://oeis.org/A022397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 27
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed = 1; /* last index with a known value */

    /* First call: allocate space for at least the first two terms */
    if (cache == NULL) {
        cache_cap = (n >= 1 ? n + 1 : 2);      /* ensure indices 0 and 1 exist */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)27;
        computed = 1;                            /* we now know a(0) and a(1) */
    }
    /* Grow the cache if needed */
    else if (n >= cache_cap) {
        idx_t new_cap = (n + 1 > cache_cap * 2) ? (n + 1) : (cache_cap * 2);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2];
        }
        computed = n;
    }

    return cache[n];
}
