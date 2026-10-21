/*
 * A224407  Number of n X 6 0..1 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) + 9*a(n-3) + 4*a(n-5) + 16*a(n-6)
 *            for n>7; initial values: a(1)=22, a(2)=169, a(3)=879, a(4)=3902,
 *            a(5)=16420, a(6)=68282, a(7)=284254
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for n=0, but return 0 safely */
    static const seq_t init[7] = {
        (seq_t)22,   (seq_t)169,   (seq_t)879,
        (seq_t)3902, (seq_t)16420, (seq_t)68282,
        (seq_t)284254
    };
    if (n <= 7) return init[n-1];

    seq_t h[7];
    for (size_t i = 0; i < 7; ++i) h[i] = init[i];

    for (idx_t idx = 8; idx <= n; ++idx) {
        seq_t a1 = h[6];   /* a(n-1) */
        seq_t a2 = h[5];   /* a(n-2) */
        seq_t a3 = h[4];   /* a(n-3) */
        seq_t a5 = h[2];   /* a(n-5) */
        seq_t a6 = h[1];   /* a(n-6) */

        /* compute using only additions and a final subtraction */
        seq_t pos = 7 * a1;
        pos = pos + 9 * a3;
        pos = pos + 4 * a5;
        pos = pos + 16 * a6;
        seq_t neg = 14 * a2;
        seq_t val = pos - neg;

        /* shift history: drop a(n-1), push new val */
        h[0] = h[1];
        h[1] = h[2];
        h[2] = h[3];
        h[3] = h[4];
        h[4] = h[5];
        h[5] = h[6];
        h[6] = val;
    }
    return h[6];
}
