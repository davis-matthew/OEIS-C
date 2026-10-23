/*
 * A037616  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,2.
 * https://oeis.org/A037616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1, a(2)=12, a(3)=110, a(4)=991,
 *            a(n)=9*a(n-1)+a(n-3)-9*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for stdint types if needed */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined offset; return 0 as a safe placeholder. */
        return (seq_t)0;
    }
    /* Quick response for the first four values. */
    switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)12;
        case 3: return (seq_t)110;
        case 4: return (seq_t)991;
        default: break;
    }

    /* Keep the last four terms only. */
    seq_t t1 = (seq_t)1;     /* a(1) */
    seq_t t2 = (seq_t)12;    /* a(2) */
    seq_t t3 = (seq_t)110;   /* a(3) */
    seq_t t4 = (seq_t)991;   /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)9 * t4 + t2 - (seq_t)9 * t1;
        /* shift the window */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = next;
    }

    return t4;  /* a(n) */
}
