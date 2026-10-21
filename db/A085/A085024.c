/*
 * A085024  Duplicate of A085001.
 * https://oeis.org/A085024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 9*n^2 + 15*n + 4
 * Range:     i8 n <= 2, i16 n <= 15, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute the quadratic polynomial safely using seq_t arithmetic */
    seq_t term1 = 9 * nn * nn;   /* 9*n^2 */
    seq_t term2 = 15 * nn;       /* 15*n   */
    return term1 + term2 + 4;    /* 9*n^2 + 15*n + 4 */
}
