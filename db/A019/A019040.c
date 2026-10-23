/*
 * A019040  Expansion of 1/((1-4*x)*(1-5*x)*(1-11*x)).
 * https://oeis.org/A019040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 119*a(n-2) + 220*a(n-3) for n≥3; * a(0)=1,
 *            a(1)=20, a(2)=281. * (Closed form: a(n) = 16*4^n/7 - 25*5^n/6 +
 *            121*11^n/42.)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values */
    static size_t   cap   = 0;    /* allocated size */
    static size_t   len   = 0;    /* number of initialised terms */

    /* ensure we have space for index n */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = (size_t)n + 1;
        if (newcap < 3) newcap = 3;          /* at least three seeds */
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* seed the first three terms once; cast truncates on overflow */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)20;
        cache[2] = (seq_t)281;
        len     = 3;
    }

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t t1 = (seq_t)20  * cache[i - 1];
        seq_t t2 = (seq_t)119 * cache[i - 2];
        seq_t t3 = (seq_t)220 * cache[i - 3];
        cache[i] = t1 - t2 + t3;               /* unsigned arithmetic */
    }
    if ((size_t)(n + 1) > len) len = (size_t)(n + 1);

    return cache[n];
}
