/*
 * A078032  Expansion of (1-x)/(1+x^2+x^3).
 * https://oeis.org/A078032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=-1, a(2)=-1, a(n) = -a(n-2) - a(n-3) for n>=3
 * Range:     i8 n <= 27, i16 n <= 57, i32 n <= 113, i64 n <= 230,
 *            i128 n <= 463, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* capacity of cache */
    static idx_t len   = 0;   /* number of values actually stored (cache[0..len-1]) */

    /* Resize cache if necessary. */
    if (n >= cap) {
        idx_t new_cap = cap ? cap : 4;
        while (new_cap <= n) new_cap <<= 1;              /* double until sufficient */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* Zero-initialise any new entries. */
        for (idx_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* Compute missing values up to n. */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0)          cache[i] = (seq_t)1;
        else if (i == 1)     cache[i] = (seq_t)-1;     /* can be written as -1 */
        else if (i == 2)     cache[i] = (seq_t)-1;
        else                cache[i] = -(cache[i-2]) - (cache[i-3]);
    }
    len = n + 1;

    return cache[n];
}
