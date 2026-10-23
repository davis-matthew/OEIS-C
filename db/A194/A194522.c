/*
 * A194522  First coordinate of (4,5)-Lagrange pair for n.
 * https://oeis.org/A194522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*n - 5*floor((7*n + 4)/9)
 * Range:     i8 n <= 17, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;                     /* convert index to seq_t */
    seq_t v    = 7*nn + 4;                     /* 7*n + 4 */
    seq_t div  = v / 9;                        /* floor division (positive) */
    return 4*nn - 5*div;                      /* 4*n - 5*floor((7*n+4)/9) */
}
