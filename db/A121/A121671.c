/*
 * A121671  Real part of (1 + n*i)^5.
 * https://oeis.org/A121671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*n^4 - 10*n^2 + 1
 * Range:     i8 n <= 2, i16 n <= 9, i32 n <= 143, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;
    seq_t n4   = n2 * n2;
    return 5 * n4 - 10 * n2 + 1;
}
