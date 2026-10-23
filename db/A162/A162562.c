/*
 * A162562  a(n) = ((5+sqrt(3))*(1+sqrt(3))^n + (5-sqrt(3))*(1-sqrt(3))^n)/2.
 * https://oeis.org/A162562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) a(0) = 5, a(1) = 8
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   alloc_seq, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq.h"
#include "../utils/grow_seq.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    /* use a static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored           */
    static size_t cache_cap = 0;   /* allocated capacity                  */

    /* first call initialisation */
    if (cache == NULL) {
        /* start with the two initial terms */
        cache_len = 2;
        cache_cap = 2;
        cache = alloc_seq(cache_len);
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)8;
    }

    /* enlarge cache if necessary */
    if (n >= cache_len) {
        /* grow capacity to at least n+1 */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */
        grow_seq(&cache, &cache_cap, new_cap);

        /* compute missing terms using the recurrence */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* a(i) = 2 * a(i-1) + 2 * a(i-2) */
            cache[i] = (seq_t)2 * cache[i-1] + (seq_t)2 * cache[i-2];
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
