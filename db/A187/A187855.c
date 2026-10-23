/*
 * A187855  Number of 7-step king-knight's tours (piece capable of both kinds
 *          of moves) on an n X n board summed over all starting positions.
 * https://oeis.org/A187855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9862808*n^2 - 82444808*n + 168212080 for n>11 For n <= 11
 *            the values are hard‑coded from the OEIS b-file
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 25, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    /* values for n = 1 .. 11 */
    static const seq_t table[12] = {
        0,          /* dummy 0‑th element, unused */
        0,          /* a(1) */
        0,          /* a(2) */
        35328,      /* a(3) */
        1533064,    /* a(4) */
        10353632,   /* a(5) */
        33908456,   /* a(6) */
        76860784,   /* a(7) */
        140714528,  /* a(8) */
        225295032,  /* a(9) */
        330074144,  /* a(10) */
        454720992   /* a(11) */
    };
    if (n <= 11) return table[n];

    /* n > 11 : use quadratic polynomial */
    seq_t ns = (seq_t)n;
    seq_t result = (seq_t)9862808 * ns * ns
                 - (seq_t)82444808 * ns
                 + (seq_t)168212080;
    return result;
}
