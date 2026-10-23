/*
 * A155120  a(n) = 2*(n^3 + n^2 + n - 1).
 * https://oeis.org/A155120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*(n^3 + n^2 + n - 1)
 * Range:     i8 n <= 3, i16 n <= 25, i32 n <= 1023, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    return 2 * (n3 + n2 + N - 1);
}
