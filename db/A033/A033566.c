/*
 * A033566  a(n) = (2*n+1) * (4*n-1).
 * https://oeis.org/A033566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n+1) * (4*n-1)
 * Range:     i8 n <= 3, i16 n <= 63, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return (2 * N + 1) * (4 * N - 1);
}
