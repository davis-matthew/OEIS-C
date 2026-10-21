/*
 * A231377  Number of n X 3 0..1 arrays with no element less than a strict
 *          majority of its horizontal and vertical neighbors.
 * https://oeis.org/A231377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 11*a(n-2) + 16*a(n-3) - 14*a(n-4)
           - 3*a(n-5) + 8*a(n-6) - 9*a(n-7) - 2*a(n-8) + 6*a(n-9) + 2*a(n-10)
   Initial values: a(1)=4, a(2)=21, a(3)=93, a(4)=378, a(5)=1519,
   a(6)=6126, a(7)=24747, a(8)=99964, a(9)=403743, a(10)=1630662,
   a(11)=6586087.  This recurrence holds for n>11. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    static bool init = false;
    static seq_t a0[11];   /* a0[0] stores a(1) */

    if (!init) {
        a0[0]  = 4;          /* a(1) */
        a0[1]  = 21;         /* a(2) */
        a0[2]  = 93;         /* a(3) */
        a0[3]  = 378;        /* a(4) */
        a0[4]  = 1519;       /* a(5) */
        a0[5]  = 6126;       /* a(6) */
        a0[6]  = 24747;      /* a(7) */
        a0[7]  = 99964;      /* a(8) */
        a0[8]  = 403743;     /* a(9) */
        a0[9]  = 1630662;    /* a(10) */
        a0[10] = 6586087;    /* a(11) */
        init = true;
    }

    if (n <= 11)
        return a0[n - 1];

    /* Sliding window of the last 11 terms */
    seq_t win[11];
    for (int i = 0; i < 11; ++i)
        win[i] = a0[i];

    for (idx_t k = 12; k <= n; ++k) {
        seq_t next =
              6 * win[10]          /* a(k-1) */
            - 11 * win[9]           /* a(k-2) */
            + 16 * win[8]           /* a(k-3) */
            - 14 * win[7]           /* a(k-4) */
            - 3 * win[6]            /* a(k-5) */
            + 8 * win[5]            /* a(k-6) */
            - 9 * win[4]            /* a(k-7) */
            - 2 * win[3]            /* a(k-8) */
            + 6 * win[2]            /* a(k-9) */
            + 2 * win[1];           /* a(k-10) */

        /* shift window left by one position */
        for (int i = 0; i < 10; ++i)
            win[i] = win[i + 1];
        win[10] = next;
    }

    return win[10];
}
