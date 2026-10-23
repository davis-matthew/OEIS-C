/*
 * A135432  a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) - a(n-5) with a(0)=0,
 *          a(1)=1, a(2)=2, a(3)=3 and a(4)=4.
 * https://oeis.org/A135432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 71, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) - a(n-5)
   with a(0)=0, a(1)=1, a(2)=2, a(3)=3, a(4)=4 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;    /* allocated size (number of elements) */
    static idx_t cached_max = 0;   /* largest index already computed */

    /* initial values */
    const seq_t init[5] = { (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)4 };

    /* allocate / grow cache if needed */
    if (cache == NULL) {
        /* allocate at least up to the requested index or 5, whichever is larger */
        idx_t needed = (n >= 4) ? (n + 1) : 5;
        cache_cap = needed;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory, return 0 */

        /* initialize base values 0..4 if within range */
        for (idx_t i = 0; i < 5 && i < cache_cap; ++i) {
            cache[i] = init[i];
        }
        cached_max = (cache_cap > 0) ? cache_cap - 1 : 0;
    } else if (n + 1 > cache_cap) {
        /* grow cache to accommodate n */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out of memory, return 0 */
        cache = tmp;
        cache_cap = new_cap;
        /* initialize any newly added elements up to 4 with base values */
        for (idx_t i = 5; i <= cached_max && i < cache_cap; ++i) {
            /* nothing to do; they are already computed */
        }
    }

    /* If the requested index is already computed, return it */
    if (n <= cached_max) {
        return cache[n];
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i < 5) {
            cache[i] = init[i];
        } else {
            /* use unsigned arithmetic; the recurrence guarantees non‑negative result */
            seq_t sum = cache[i - 1] + cache[i - 2] +
                        cache[i - 3] + cache[i - 4];
            cache[i] = sum - cache[i - 5];
        }
    }
    cached_max = n;

    return cache[n];
}
