/*
 * A075352  Group the natural numbers so that the product of the terms of the
 *          n-th group is just >= n!: (1), (2), (3, 4), (5, 6), (7, 8, 9),
 *          (10, 11, 12), (13, 14, 15, 16), (17, 18, 19, 20), (21, 22, 23, 24,
 *          25), (26, 27, 28, 29, 30), ... Sequence gives product of numbers
 *          in each group.
 * https://oeis.org/A075352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Groups natural numbers so that the product of each consecutive group
 * is the smallest product >= n!.  Returns the product of the n‑th group
 * (OEIS A075352).  This implementation keeps a static cache so that
 * calls with arbitrary n are answered correctly in O(n) time.
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state persisting between calls */
    static seq_t *cache     = NULL; /* cached a(k) for k=1..cache_len */
    static size_t cache_len = 0;    /* number of terms cached          */
    static size_t cache_cap = 0;    /* allocated capacity              */
    static idx_t  next_start = 1;   /* first unused natural number     */
    static seq_t  fact       = (seq_t)1; /* factorial of cache_len */

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache array if needed */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : n;
        if (new_cap < (size_t)n) new_cap = n;
        /* Cast realloc result so it works in both C and C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute a(k) for k = cache_len+1 .. n */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* Update factorial for current k  (k! = (k-1)! * k) */
        fact *= (seq_t)k;

        /* Find next group whose product >= k! */
        seq_t prod = (seq_t)1;
        for (idx_t m = next_start; ; ++m) {
            prod *= (seq_t)m;
            if (prod >= fact) {
                cache[k - 1] = prod;
                next_start = m + 1;
                break;
            }
        }
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
