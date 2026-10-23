/*
 * A041011  Denominators of continued fraction convergents to sqrt(8).
 * https://oeis.org/A041011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4) for n >= 4 with initial values a(0)=1,
 *            a(1)=1, a(2)=5, a(3)=6
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for explicit integer types; seq_t/idx_t are supplied */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)5;
        case 3: return (seq_t)6;
        default: break;
    }

    /* iterate using the 4‑term recurrence.
       a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1)  */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)5;   /* a(2) */
    seq_t a3 = (seq_t)6;   /* a(3) */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)6 * a2 - a0;   /* a(i) = 6*a(i-2) - a(i-4) */
        /* rotate the sliding window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return ai;
}
