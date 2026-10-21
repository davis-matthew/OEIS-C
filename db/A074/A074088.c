/*
 * A074088  Coefficient of q^2 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(2,3).
 * https://oeis.org/A074088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Sequence A074088 via recurrence a(0)=0, a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=21, a(5)=120, a(6)=585, a(7)=2508
 *            a(n)=6*a(n-1)-3*a(n-2)-28*a(n-3)+9*a(n-4)+54*a(n-5)+27*a(n-6)
 *            (n>=8) Reference: OEIS A074088
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0 .. 7 */
    static const seq_t base[8] = {
        0,   /* a(0) */
        0,   /* a(1) */
        0,   /* a(2) */
        0,   /* a(3) */
        21,  /* a(4) */
        120, /* a(5) */
        585, /* a(6) */
        2508 /* a(7) */
    };

    /* For indices below 8, return the hard‑coded value */
    if (n <= 7) {
        return base[n];
    }

    /* For n >= 8 compute using the recurrence.
       We keep the last 6 values in a circular buffer.  */
    seq_t last[6];

    /* Initialise with a(2)..a(7) which correspond to n-6 .. n-1 for n=8 */
    last[0] = base[2];  /* a(n-6) */
    last[1] = base[3];  /* a(n-5) */
    last[2] = base[4];  /* a(n-4) */
    last[3] = base[5];  /* a(n-3) */
    last[4] = base[6];  /* a(n-2) */
    last[5] = base[7];  /* a(n-1) */

    seq_t current = 0;
    idx_t i;

    for (i = 8; i <= n; ++i) {
        /* Compute a(i) using the recurrence */
        current = 6 * last[5]
                - 3 * last[4]
                - 28 * last[3]
                + 9 * last[2]
                + 54 * last[1]
                + 27 * last[0];

        /* Shift the window: drop a(n-6), shift others, append new */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = current;
    }

    return current;
}
