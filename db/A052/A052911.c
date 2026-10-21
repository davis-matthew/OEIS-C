/*
 * A052911  Expansion of (1-x)/(1 - 3*x - x^2 + 2*x^3).
 * https://oeis.org/A052911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 2*a(n-3) (n>=3), a(0)=1, a(1)=2,
 *            a(2)=7
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Ensure cache can hold values up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        /* realloc may return NULL only if needed == 0, which cannot happen here */
        cache = new_cache;
        /* Fill missing initial terms if required */
        if (cache_size == 0) {
            cache[0] = 1;                 /* a(0) */
            if (needed > 1) cache[1] = 2; /* a(1) */
            if (needed > 2) cache[2] = 7; /* a(2) */
            cache_size = (needed > 2) ? 3 : needed;
        } else if (cache_size == 1) {
            if (needed > 1) cache[1] = 2; /* a(1) */
            if (needed > 2) cache[2] = 7; /* a(2) */
            cache_size = (needed > 2) ? 3 : needed;
        } else if (cache_size == 2) {
            cache[2] = 7;                /* a(2) */
            cache_size = 3;
        }
        /* Compute remaining terms up to needed-1 using the recurrence */
        for (size_t i = cache_size; i < needed; ++i) {
            seq_t term = cache[i - 1];
            term = term * 3 + cache[i - 2];
            term = term - cache[i - 3] * 2;
            cache[i] = term;
        }
        cache_size = needed;
    }
    return cache[n];
}
