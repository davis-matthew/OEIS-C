/*
 * A193048  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A193048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            6*a(n-1)-14*a(n-2)+15*a(n-3)-5*a(n-4)-4*a(n-5)+4*a(n-6)-a(n-7),
 *            * with a(0)=1, a(1)=0, a(2)=1, a(3)=2, a(4)=8, a(5)=25, a(6)=68,
 *            a(7)=163
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;       /* memoised sequence values */
    static size_t cap = 0;            /* capacity of allocated array */
    static size_t len = 0;            /* number of values currently cached */
    /* initial seven terms (offset 0) */
    static const seq_t init[8] = { 1, 0, 1, 2, 8, 25, 68, 163 };

    /* fast exit for low indices */
    if (n < 7) return init[n];

    /* ensure storage can hold index n */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = cap ? cap << 1 : 8;
        if (newcap < (size_t)(n + 1)) newcap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* make sure the first eight values are cached */
    if (len < 8) {
        for (size_t i = len; i < 8; ++i) cache[i] = init[i];
        len = 8;
    }

    /* compute missing entries up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t t1 = cache[i-1] * 6 + cache[i-3] * 15 + cache[i-6] * 4;
        seq_t t2 = cache[i-2] * 14 + cache[i-4] * 5 + cache[i-5] * 4 + cache[i-7];
        cache[i] = t1 - t2;   /* t1 ≥ t2 by the nature of the sequence */
    }
    len = (size_t)n + 1;
    return cache[(size_t)n];
}
