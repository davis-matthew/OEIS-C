/*
 * A099814  Bisection of A002275.
 * https://oeis.org/A099814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * 100 + 11 (a(0) = 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated elements   */
    static size_t sz    = 0;   /* valid elements count */

    if (n >= sz) {
        /* Grow cache to at least n+1 elements */
        size_t needed = (size_t)n + 1;
        if (cap < needed) {
            size_t new_cap = cap ? cap * 2 : 4;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            /* realloc failure is ignored – the build environment guarantees enough memory */
            cache = tmp;
            cap = new_cap;
        }
        /* Initialise new entries */
        for (size_t i = sz; i < needed; ++i) {
            if (i == 0)
                cache[i] = 0;
            else
                cache[i] = cache[i - 1] * 100 + 11;
        }
        sz = needed;
    }
    return cache[n];
}
