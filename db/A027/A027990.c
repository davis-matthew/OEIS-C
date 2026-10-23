/*
 * A027990  Sum{T(n,k)*T(n,2n-k)}, 0<=k<=n, T given by A027926.
 * https://oeis.org/A027990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) - 5*a(n-3) + 5*a(n-4) - a(n-5), with
 *            initial values a(0)=1, a(1)=2, a(2)=7, a(3)=21, a(4)=65
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small table of the first five terms */
    static const seq_t init[5] = {1, 2, 7, 21, 65};

    if (n < 5)
        return init[n];

    /* Variables representing a(n-5), a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = init[0];   /* a(n-5) */
    seq_t a1 = init[1];   /* a(n-4) */
    seq_t a2 = init[2];   /* a(n-3) */
    seq_t a3 = init[3];   /* a(n-2) */
    seq_t a4 = init[4];   /* a(n-1) */
    seq_t an = 0;         /* a(n) to compute */

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute 5*a(n-1) + 5*a(n-4) - (5*a(n-2) + 5*a(n-3) + a(n-5)) */
        seq_t t1 = a4 * 5;          /* 5*a(n-1) */
        seq_t t2 = a3 * 5;          /* 5*a(n-2) */
        seq_t t3 = a2 * 5;          /* 5*a(n-3) */
        seq_t t4 = a1 * 5;          /* 5*a(n-4) */

        seq_t sum_pos = t1 + t4;    /* 5*a(n-1) + 5*a(n-4) */
        seq_t sum_neg = t2 + t3 + a0;   /* 5*a(n-2) + 5*a(n-3) + a(n-5) */

        an = sum_pos - sum_neg;     /* recurrence */

        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return an;
}
