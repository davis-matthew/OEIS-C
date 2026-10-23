/*
 * A194517  Second coordinate of (3,5)-Lagrange pair for n.
 * https://oeis.org/A194517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n - 3*floor((5*n + 4)/8)
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; value for n==0 is irrelevant */
    seq_t N = (seq_t)n;
    seq_t t = ((seq_t)5 * N + 4) / 8; /* floor division of positive integers */
    return 2 * N - 3 * t;
}
