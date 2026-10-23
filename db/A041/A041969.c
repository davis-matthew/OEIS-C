/*
 * A041969  Denominators of continued fraction convergents to sqrt(507).
 * https://oeis.org/A041969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2702*a(n-6) - a(n-12) for n > 11 Initial values (n = 0 ..
 *            11): a(0)=1, a(1)=1, a(2)=2, a(3)=29, a(4)=31, a(5)=60,
 *            a(6)=2671, a(7)=2731, a(8)=5402, a(9)=78359, a(10)=83761,
 *            a(11)=162120
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS A041969 */
seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Pre‑computed initial 12 terms */
    static const seq_t init[12] = {
        (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)29,
        (seq_t)31, (seq_t)60, (seq_t)2671,(seq_t)2731,
        (seq_t)5402,(seq_t)78359,(seq_t)83761,(seq_t)162120
    };

    /* Allocate and prime the cache on first call */
    if (cache == NULL) {
        cache_cap = (n < 64) ? 64 : n + 1;          /* small initial capacity */
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        /* copy the initial 12 terms */
        for (size_t i = 0; i < 12; ++i) cache[i] = init[i];
        cache_len = 12;
    }

    /* Grow the cache if required */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1; /* double until large enough */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* Compute terms up to n, storing them in the cache */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 11) {
            val = init[i];                     /* initial values */
        } else {
            /* recurrence: a(n) = 2702*a(n-6) - a(n-12) */
            val = (seq_t)2702 * cache[i - 6] - cache[i - 12];
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
