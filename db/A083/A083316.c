/*
 * A083316  a(n) = (2*6^n - (5^n - 3^n))/2.
 * https://oeis.org/A083316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*6^n - (5^n - 3^n))/2 Equivalently, a(0) = 1, a(1) = 5,
 *            a(2) = 28, a(n) = 14*a(n-1) - 63*a(n-2) + 90*a(n-3) for n >= 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary‑order calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of computed terms */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initial allocation */
    if (cache_cap == 0) {
        cache_cap = 8;                     /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* grow the cache if needed */
    while (cache_len <= n) {
        if (cache_len == 0) {
            cache[0] = 1;                  /* a(0) */
        } else if (cache_len == 1) {
            cache[1] = 5;                  /* a(1) */
        } else if (cache_len == 2) {
            cache[2] = 28;                 /* a(2) */
        } else {
            /* recurrence: a(n) = 14*a(n-1) - 63*a(n-2) + 90*a(n-3) */
            size_t k = cache_len;
            seq_t val = (seq_t)(14) * cache[k-1]
                       - (seq_t)(63) * cache[k-2]
                       + (seq_t)(90) * cache[k-3];
            cache[k] = val;
        }
        cache_len++;

        /* resize when capacity is insufficient */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;                /* double the capacity */
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
    }
    return cache[n];
}
