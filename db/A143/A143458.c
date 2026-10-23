/*
 * A143458  Expansion of 1/(x^k*(1-x-3*x^(k+1))) for k=7.
 * https://oeis.org/A143458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 32, u32 n <= 69, u64 n <= 142,
 *            u128 n <= 287, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) + 3*a(n-8);  a(0)=1, a(1)=4, a(2)=7, a(3)=10, a(4)=13,
   a(5)=16, a(6)=19, a(7)=22 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements currently cached */
    static const seq_t init[8] = {1, 4, 7, 10, 13, 16, 19, 22};

    /* If already computed, return immediately */
    if (n < cache_size)
        return cache[n];

    /* Remember old size before we possibly reallocate. */
    idx_t old_size = cache_size;

    /* Grow cache to hold indices up to n */
    idx_t new_size = n + 1;               /* elements needed [0..n] */
    size_t alloc_bytes = (size_t)new_size * sizeof(seq_t);

    if (cache == NULL) {
        cache = (seq_t *)malloc(alloc_bytes);
    } else {
        cache = (seq_t *)realloc(cache, alloc_bytes);
    }
    cache_size = new_size;

    /* Compute missing values, starting from old_size */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i < 8)
            cache[i] = init[i];
        else
            cache[i] = cache[i - 1] + (seq_t)3 * cache[i - 8];
    }

    return cache[n];
}
