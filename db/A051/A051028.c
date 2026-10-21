/*
 * A051028  Ramanujan's a-series: expansion of (1+53x+9x^2)/(1-82x-82x^2+x^3).
 * https://oeis.org/A051028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 82*a(n-1) + 82*a(n-2) - a(n-3) with a(0)=1, a(1)=135,
 *            a(2)=11161 (interpretation of the generating function
 *            (1+53x+9x^2)/((1+x)(1-83x+x^2)))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute the n-th term of OEIS A051028 */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static idx_t sz     = 0;          /* number of stored terms   */
    static idx_t cap    = 0;          /* allocated capacity       */
    static seq_t *buf   = NULL;       /* dynamic array storage    */

    /* ensure enough capacity */
    if (cap < n+1) {
        idx_t newcap = cap ? cap*2 : 8;
        while (newcap <= n) newcap *= 2;
        buf = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* initialise the first three terms if they are missing */
    if (sz == 0) {
        buf[0] = (seq_t)1;                         /* a(0) */
    }
    if (sz <= 1) {
        buf[1] = (seq_t)53 + ((seq_t)82 * buf[0]); /* a(1) */
    }
    if (sz <= 2) {
        buf[2] = (seq_t)9 + ((seq_t)82 * buf[1]) + ((seq_t)82 * buf[0]); /* a(2) */
    }

    /* compute any remaining terms up to n */
    for (idx_t i = sz; i <= n; ++i) {
        if (i < 3) continue;   /* already initialised */
        buf[i] = buf[i-1] * (seq_t)82
               + buf[i-2] * (seq_t)82
               - buf[i-3];
    }

    sz = n + 1;   /* cache now contains terms 0..n */
    return buf[n];
}
