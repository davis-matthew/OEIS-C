/*
 * A153265  a(n) = (-2*I)^n + (2*I)^n + (1/2 + 1/2*I*sqrt(3))^n + (1/2 -
 *          1/2*I*sqrt(3))^n.
 * https://oeis.org/A153265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - 5*a(n-2) + 4*a(n-3) - 4*a(n-4) with initial
 *            terms: a(0)=4, a(1)=1, a(2)=-9, a(3)=-2
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 31, i64 n <= 61, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t init[4] = { (seq_t)4, (seq_t)1, (seq_t)-9, (seq_t)-2 };

    /* Cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated capacity (number of elements) */
    static idx_t last   = 3;      /* largest index already computed in cache */

    if (n < 4) {
        return init[n];
    }

    /* Ensure the cache has enough space for indices up to n */
    if (cache == NULL) {
        /* Allocate initial space for indices 0..3 */
        size_t init_cap = 4;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(1);
        for (size_t i = 0; i < init_cap; i++) {
            cache[i] = init[i];
        }
        cap = init_cap;
    }

    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) {
            newcap = newcap > 0 ? newcap * 2 : 8;
        }
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        cache[i] = a1 - (seq_t)5 * a2 + (seq_t)4 * a3 - (seq_t)4 * a4;
    }
    last = n;

    return cache[n];
}
