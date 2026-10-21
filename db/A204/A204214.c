/*
 * A204214  Number of length 6 nonnegative integer arrays starting and ending
 *          with 0 with adjacent elements differing by no more than n.
 * https://oeis.org/A204214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (23*n^4 + 74*n^3 + 91*n^2 + 52*n + 12)/12
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 135, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Adjusted from the OEIS definition: a(n) = (23/12)n^4 + (37/6)n^3 + (91/12)n^2 + (13/3)n + 1) */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Perform all calculations in seq_t to avoid overflow of native types. */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n4 = n2 * n2;          /* n^4 */
    seq_t n3 = n2 * nn;          /* n^3 */

    seq_t term1 = (seq_t)23 * n4; /* 23*n^4 */
    seq_t term2 = (seq_t)74 * n3; /* 74*n^3   (74 = 2*37) */
    seq_t term3 = (seq_t)91 * n2; /* 91*n^2   (91 = 91) */
    seq_t term4 = (seq_t)52 * nn; /* 52*n     (52 = 4*13) */

    seq_t numer = term1 + term2 + term3 + term4 + (seq_t)12;
    return numer / (seq_t)12;
}
