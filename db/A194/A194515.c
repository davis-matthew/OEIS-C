/*
 * A194515  Second coordinate of (3,4)-Lagrange pair for n.
 * https://oeis.org/A194515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n - 3*floor((2*n + 3)/7)
 * Range:     i8 n <= 62, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to seq_t for arithmetic */
    seq_t t  = sn * 2 + 3;        /* 2*n + 3  */
    seq_t tdiv = t / 7;           /* floor((2*n + 3)/7)   */
    return sn - 3 * tdiv;         /* n - 3*floor(...)     */
}
