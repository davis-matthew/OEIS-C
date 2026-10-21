/*
 * A077290  Triangular numbers that are 6 times other triangular numbers.
 * https://oeis.org/A077290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*T(p) where the pair (X,Y) = (2p+1,2q+1) satisfies * the
 *            Pell equation X^2 - 6*Y^2 = -5. * * Two interleaved sequences of
 *            solutions to this equation are generated * by the recurrence * *
 *            X' = 5*X + 12*Y * Y' = 2*X + 5*Y * * starting from the initial
 *            pairs (1,1) and (7,3). These produce al...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) {                         /* first term is 0 */
        return (seq_t)0;
    }

    /* State for the first class of solutions (starting from 1,1). */
    seq_t XA = (seq_t)1, YA = (seq_t)1;
    seq_t nextXA = (seq_t)5 * XA + (seq_t)12 * YA;
    seq_t nextYA = (seq_t)2 * XA + (seq_t)5 * YA;

    /* State for the second class of solutions (starting from 7,3). */
    seq_t XB = (seq_t)7, YB = (seq_t)3;
    seq_t nextXB = (seq_t)5 * XB + (seq_t)12 * YB;
    seq_t nextYB = (seq_t)2 * XB + (seq_t)5 * YB;

    seq_t result = 0;                     /* will hold the last computed term */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t Xsel;                       /* selected X for this term */

        if (nextXA <= XB) {
            /* Take next from class A. */
            Xsel = nextXA;

            /* Advance class A to its next value. */
            XA = nextXA;
            YA = nextYA;
            nextXA = (seq_t)5 * XA + (seq_t)12 * YA;
            nextYA = (seq_t)2 * XA + (seq_t)5 * YA;
        } else {
            /* Take next from class B. */
            Xsel = XB;

            /* Advance class B to its next value. */
            XB = nextXB;
            YB = nextYB;
            nextXB = (seq_t)5 * XB + (seq_t)12 * YB;
            nextYB = (seq_t)2 * XB + (seq_t)5 * YB;
        }

        /* Compute the triangular number from the selected X. */
        seq_t p = (Xsel - (seq_t)1) / (seq_t)2;
        result = p * (p + (seq_t)1) / (seq_t)2;
    }

    return result;
}
