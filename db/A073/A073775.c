/*
 * A073775  Polynomial (1/3)*n^3 + (9/2)*n^2 + (85/6)*n - 2.
 * https://oeis.org/A073775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/3)*n^3 + (9/2)*n^2 + (85/6)*n - 2
 * Range:     i8 n <= 1, i16 n <= 21, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;          /* n as seq_t for integer arithmetic */
    seq_t n2   = nn * nn;           /* n² */
    seq_t n3   = n2 * nn;           /* n³ */

    /* compute (2*n³ + 27*n² + 85*n)/6 - 2 using only integer ops */
    seq_t num  = 2 * n3 + 27 * n2 + 85 * nn;
    seq_t res  = num / 6 - 2;
    return res;
}
