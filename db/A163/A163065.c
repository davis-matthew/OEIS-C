/*
 * A163065  a(n) = ((3+sqrt(5))*(5+sqrt(5))^n + (3-sqrt(5))*(5-sqrt(5))^n)/2.
 * https://oeis.org/A163065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 20*a(n-2); a(0) = 3, a(1) = 20
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_sz  = 0;

    /* initialize cache for n=0 and n=1 */
    if (cache_sz == 0) {
        size_t sz = 2;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* 0 in case of allocation failure */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)20;
        cache_sz = 2;
        cache_cap = sz;
    }

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_sz) {
        size_t required = (size_t)n + 1;
        if (required > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            if (new_cap < required) new_cap = required;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;        /* 0 in case of allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms using the recurrence */
        for (size_t k = cache_sz; k <= (size_t)n; ++k) {
            seq_t a1 = cache[k-1];
            seq_t a2 = cache[k-2];
            seq_t val = (seq_t)10 * a1 - (seq_t)20 * a2;
            cache[k] = val;
        }
        cache_sz = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
