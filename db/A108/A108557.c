/*
 * A108557  Row sums of triangle A108556, in which row n equals the inverse
 *          binomial transform of the crystal ball sequence for D_n lattice.
 * https://oeis.org/A108557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 12*a(n-1) - 46*a(n-2) + 60*a(n-3) - 9*a(n-4)
   for n>=6; initial values: a(0)=1, a(1)=3, a(2)=9, a(3)=63,
   a(4)=433, a(5)=2823 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 5) {
        /* cover the initial five terms directly */
        switch (n) {
            case 0: return 1;
            case 1: return 3;
            case 2: return 9;
            case 3: return 63;
            case 4: return 433;
            case 5: return 2823;
            default: return 0;  /* unreachable */
        }
    }

    /* values for a(n-4)..a(n-1) when n==6 */
    seq_t a2 = 9;          /* a(2) */
    seq_t a3 = 63;         /* a(3) */
    seq_t a4 = 433;        /* a(4) */
    seq_t a5 = 2823;       /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t term1 = 12 * a5;      /* 12*a(n-1) */
        seq_t term2 = 46 * a4;      /* 46*a(n-2) */
        seq_t term3 = 60 * a3;      /* 60*a(n-3) */
        seq_t term4 = 9 * a2;       /* 9*a(n-4)  */

        seq_t a_next = term1 - term2 + term3 - term4;

        /* shift the sliding window */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a_next;
    }

    return a5;
}
