/*
 * A208207  a(n)=(a(n-1)^3*a(n-2)+1)/a(n-3) with a(0)=a(1)=a(2)=1.
 * https://oeis.org/A208207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)^3*a(n-2)+1)/a(n-3), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;     /* array of a(k) values      */
    static size_t cap   = 0;        /* allocated capacity        */
    static size_t len   = 0;        /* number of computed terms  */

    /* initialise cache if required */
    if (cache == NULL) {
        cap = 3;
        if (cap < 3) cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = 1;
        len = 3;
    }

    /* grow capacity if n is outside current allocation */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;              /* double until sufficient */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute terms up to n (inclusive) if not already done */
    if ((size_t)n >= len) {
        for (idx_t i = (idx_t)len; i <= n; ++i) {
            seq_t a0 = cache[i - 3];   /* a(i-3) */
            seq_t a1 = cache[i - 2];   /* a(i-2) */
            seq_t a2 = cache[i - 1];   /* a(i-1) */
            seq_t numerator = a2 * a2 * a2 * a1 + 1;
            cache[i] = numerator / a0;
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
