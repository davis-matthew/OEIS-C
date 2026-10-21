/*
 * A233391  List of values of d in infinite family of points on Mordell curve
 *          x^3-y^2=d over quadratic field sqrt(13).
 * https://oeis.org/A233391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 59319*(2*t(n)-1) where t(1)=13, t(2)=23377, t(3)=30349813
 *            t(n) = 1299*t(n-1) - 1299*t(n-2) + t(n-3) (n ≥ 4)
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation of t(n) */
    static seq_t *t = NULL;
    static size_t tsize = 0;   /* number of t values stored */
    static size_t tcap   = 0;   /* capacity of the array */

    if (n == 0)
        return 0;   /* undefined for n==0, safety */

    size_t idx = (size_t)n;   /* indices 1.. */

    /* initialise on first call */
    if (tsize == 0) {
        tcap = (idx > 3 ? idx : 3);
        t = (seq_t*)malloc(tcap * sizeof(seq_t));
        if (!t) return 0;   /* allocation failed */

        /* first three terms */
        t[0] = (seq_t)13;
        t[1] = (seq_t)23377;
        t[2] = (seq_t)30349813;
        tsize = 3;
    }

    /* grow array if needed */
    if (idx > tcap) {
        size_t newcap = tcap;
        while (newcap < idx) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(t, newcap * sizeof(seq_t));
        if (!tmp) return 0;   /* reallocation failed */
        t = tmp;
        tcap = newcap;
    }

    /* compute missing terms */
    if (idx > tsize) {
        for (size_t i = tsize; i < idx; ++i) {
            t[i] = (seq_t)1299 * t[i-1] - (seq_t)1299 * t[i-2] + t[i-3];
        }
        tsize = idx;
    }

    /* a(n) = 59319 * (2*t(n) - 1) */
    seq_t val = (seq_t)59319 * ( (seq_t)2 * t[idx-1] - (seq_t)1 );
    return val;
}
