/*
 * A234590  Number of binary words of length n which have no 0^b 1 1 0^a 1 0 1
 *          0^b - matches, where a=0, b=2.
 * https://oeis.org/A234590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-7) + a(n-8) + a(n-9) for n ≥ 9. For 1 ≤ n
 *            ≤ 8: a(n) = 2*a(n-1). Initial value: a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;   /* array of seq_t */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t  cache_len = 0;  /* highest n that has been computed */

    /* On first call initialise cache with a(0)=1. */
    if (cache == NULL) {
        cache_cap = 1;
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0]  = (seq_t)1;       /* a(0) = 1 */
        cache_len = 0;
    }

    /* Return already known value. */
    if (n <= (idx_t)cache_len)
        return cache[(size_t)n];

    /* Grow cache if needed. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;          /* double until enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_len+1 up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i < 9) {                     /* a(n) = 2*a(n-1) for 1≤n≤8 */
            val = (seq_t)2 * cache[(size_t)(i - 1)];
        } else {                         /* recurrence for n ≥ 9 */
            val = (seq_t)2 * cache[(size_t)(i - 1)];
            val = val - cache[(size_t)(i - 7)];
            val = val + cache[(size_t)(i - 8)];
            val = val + cache[(size_t)(i - 9)];
        }
        cache[(size_t)i] = val;
    }

    cache_len = n;
    return cache[(size_t)n];
}
