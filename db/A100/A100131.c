/*
 * A100131  a(n) = Sum_{k=0..floor(n/4)} binomial(n-2k, 2k)*2^(n-4k).
 * https://oeis.org/A100131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-4); Initial values: a(0)=1,
 *            a(1)=2, a(2)=4, a(3)=8
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static buffer to memoise computed terms */
    static seq_t *buf = NULL;        /* array of computed values */
    static idx_t cap = 0;            /* allocated size of buf      */
    static idx_t sz  = 0;            /* number of values filled   */

    /* Initialise for the first call */
    if (cap == 0) {
        cap = 4;
        sz  = 4;
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) abort();      /* should never happen in the test harness */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)2;
        buf[2] = (seq_t)4;
        buf[3] = (seq_t)8;
    }

    /* If the requested index is already available, return it. */
    if (n < sz)
        return buf[n];

    /* Grow the buffer to accommodate n if necessary. */
    if (n + 1 > cap) {
        idx_t newcap = n + 1;
        seq_t *newbuf = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!newbuf) abort();
        buf = newbuf;
        cap = newcap;
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (idx_t i = sz; i <= n; ++i) {
        seq_t term = buf[i-1] * (seq_t)4;
        term -= buf[i-2] * (seq_t)4;
        term += buf[i-4];
        buf[i] = term;
    }
    sz = n + 1;

    return buf[n];
}
