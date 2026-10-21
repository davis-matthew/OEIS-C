/*
 * A099455  An Alexander sequence for the knot 8_12.
 * https://oeis.org/A099455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A099455: a(n) = A099454(n) - A099454(n-2)
   The sequence satisfies the linear recurrence
       a(0)=1, a(1)=7, a(2)=36, a(3)=168, a(4)=755
       a(n) = 7*a(n-1) - 13*a(n-2) + 7*a(n-3) - a(n-4)   for n>=5
   (equivalently: a(n) = 7*a(n-1) + 7*a(n-3) - (13*a(n-2) + a(n-4)))   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements actually stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Initial five values, used as pre‑computed constants. */
    static const seq_t init[5] = {1, 7, 36, 168, 755};

    if (n < 5)
        return init[n];

    /* First call: create the cache and copy the five known terms. */
    if (cache == NULL) {
        cache_cap = 16;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init[i];
        cache_len = 5;
    }

    /* Grow the cache if required. */
    if ((size_t)n >= cache_len) {
        /* Ensure enough capacity. */
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (needed > new_cap)
                new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) { /* allocation failed */
                /* In an extreme failure we return 0; the caller may
                   treat this as an error. */
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute successive terms using the recurrence. */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            cache[i] = 7 * cache[i-1] + 7 * cache[i-3]
                     - (13 * cache[i-2] + cache[i-4]);
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
