/*
 * A077420  Bisection of Chebyshev sequence T(n,3) (odd part) with Diophantine
 *          property.
 * https://oeis.org/A077420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) – a(n-2) with a(-1) = 1, a(0) = 1. For n ≥ 1
 *            a(1) = 33
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lazy memoisation:   a(0) is stored at buf[0], a(1) at buf[1], … */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: buf[i] = a(i) for 0 ≤ i < len   */
    static seq_t *buf   = NULL;
    static idx_t   len  = 0;   /* number of filled entries */
    static idx_t   cap  = 0;   /* allocated capacity */

    const seq_t ONE = (seq_t)1;
    const seq_t THIRTY_FOUR = (seq_t)34;

    /* initialise cache (only a(0) needed) */
    if (buf == NULL) {
        buf = (seq_t *)malloc(sizeof(seq_t));
        if (buf == NULL)
            return ONE;                 /* trivial fallback – allocation failure */
        buf[0] = ONE;                   /* a(0) */
        len  = 1;
        cap  = 1;
    }

    /* return cached value if already computed */
    if (n < len)
        return buf[n];

    /* grow cache to accommodate indices up to n */
    if (n >= cap) {
        idx_t newcap = (cap == 0) ? 1 : cap;
        while (newcap <= n)
            newcap <<= 1;              /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (tmp == NULL)
            return ONE;                 /* allocation failure – unlikely */
        buf = tmp;
        cap = newcap;
    }

    /* compute missing terms from current len up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t val;
        if (i == 1)                    /* use a(-1)=1 for a(1) */
            val = THIRTY_FOUR * buf[0] - ONE;
        else
            val = THIRTY_FOUR * buf[i-1] - buf[i-2];
        buf[i] = val;
    }
    len = n + 1;

    return buf[n];
}
