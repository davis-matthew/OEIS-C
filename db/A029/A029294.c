/*
 * A029294  Expansion of 1/((1-x^3)*(1-x^6)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A029294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-3)+a(n-6)+a(n-7)-a(n-9)-2*a(n-13)-a(n-17)+a(n-19)+a(n-20)+a(n-23)-a(n-26)
 *            with initial values a(0)=1, a(1)=0, a(2)=0 (all other a(k)=0 for
 *            3≤k<3)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Grow cache if needed */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        /* realloc works for both C and C++ (returns void*) */
        void *ptr = realloc(cache, new_len * sizeof(seq_t));
        if (!ptr) abort();                      /* out of memory */
        seq_t *tmp = (seq_t *)ptr;
        cache = tmp;

        /* Initialise new elements – default construct if C++ */
#ifndef __cplusplus
        for (size_t i = cache_len; i < new_len; ++i)
            cache[i] = 0;
#else
        for (size_t i = cache_len; i < new_len; ++i)
            new (&cache[i]) seq_t();
#endif

        /* Compute missing values */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                cache[0] = 1;
                continue;
            }

            seq_t v = 0;
            if (i >= 3)       v += cache[i - 3];
            if (i >= 6)       v += cache[i - 6];
            if (i >= 7)       v += cache[i - 7];
            if (i >= 9)       v -= cache[i - 9];
            if (i >= 13)      v -= (seq_t)2 * cache[i - 13];
            if (i >= 17)      v -= cache[i - 17];
            if (i >= 19)      v += cache[i - 19];
            if (i >= 20)      v += cache[i - 20];
            if (i >= 23)      v += cache[i - 23];
            if (i >= 26)      v -= cache[i - 26];

            cache[i] = v;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
