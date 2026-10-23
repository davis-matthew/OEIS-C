/*
 * A136297  a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3), with a(0)=1, a(1)=3,
 *          a(2)=1.
 * https://oeis.org/A136297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3); a(0)=1, a(1)=3, a(2)=1
 * Range:     i8 n <= 10, i16 n <= 17, i32 n <= 31, i64 n <= 58, i128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache     = NULL;   /* array of values */
    static size_t  capacity = 0;      /* allocated size of cache    */
    static idx_t   last     = 0;      /* highest index computed so far */

    /* initialise cache */
    if (cache == NULL) {
        capacity = 3;                     /* at least for indices 0..2 */
        cache    = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) exit(1);              /* fatal, should not happen */
        cache[0] = 1;                     /* a(0) */
        cache[1] = 3;                     /* a(1) */
        cache[2] = 1;                     /* a(2) */
        last     = 2;                     /* highest known index */
    }

    /* grow cache and compute any missing values up to n */
    if (n > last) {
        size_t need = (size_t)(n + 1);
        if (need > capacity) {
            size_t newcap = capacity;
            while (newcap < need) newcap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache  = tmp;
            capacity = newcap;
        }
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            cache[i] = (seq_t)3 * t1 - (seq_t)3 * t2 + (seq_t)3 * t3;
        }
        last = n;
    }
    return cache[n];
}
