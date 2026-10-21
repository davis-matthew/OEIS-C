/*
 * A229753  Number of defective 4-colorings of an n X 6 0..3 array connected
 *          horizontally, vertically, diagonally and antidiagonally with
 *          exactly two mistakes, and colors introduced in row-major 0..3
 *          order.
 * https://oeis.org/A229753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 20, u64 n <= 50, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A229753 – defective 4‐colorings with two mistakes.
   Recurrence (valid for n>11):
   a(n) = 9*a(n-1) - 33*a(n-2) + 63*a(n-3) - 66*a(n-4) + 36*a(n-5) - 8*a(n-6)
   Initial values:
   a(1)=  50, a(2)=  6456, a(3)= 17404, a(4)= 32548, a(5)= 57677,
   a(6)=102271, a(7)=186396, a(8)=354509, a(9)=704530, a(10)=1450667,
   a(11)=3059672
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Constant initial values (1‑indexed).  init[0] is unused. */
    static const seq_t init[12] = {
        0ULL,
        50ULL,          /* 1 */
        6456ULL,        /* 2 */
        17404ULL,       /* 3 */
        32548ULL,       /* 4 */
        57677ULL,       /* 5 */
        102271ULL,      /* 6 */
        186396ULL,      /* 7 */
        354509ULL,      /* 8 */
        704530ULL,      /* 9 */
        1450667ULL,     /* 10 */
        3059672ULL      /* 11 */
    };

    /* For n <= 11, return the known value. */
    if (n <= 11) return init[n];

    /* Slide a 6‑term window over the recurrence.  Starting with
       a(6) … a(11). */
    seq_t a1 = init[6];   /* a(n-6) */
    seq_t a2 = init[7];   /* a(n-5) */
    seq_t a3 = init[8];   /* a(n-4) */
    seq_t a4 = init[9];   /* a(n-3) */
    seq_t a5 = init[10];  /* a(n-2) */
    seq_t a6 = init[11];  /* a(n-1) */

    for (idx_t i = 12; i <= n; ++i) {
        seq_t next = 9 * a6
                   - 33 * a5
                   + 63 * a4
                   - 66 * a3
                   + 36 * a2
                   - 8  * a1;

        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
