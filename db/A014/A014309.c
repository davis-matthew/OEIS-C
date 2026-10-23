/*
 * A014309  a(n) = (n+2)*(n+1)*(n^2 + 7*n - 12)/24.
 * https://oeis.org/A014309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n+2)*(n+1)*(n^2 + 7*n - 12)/24
 * Range:     i8 n <= 2, i16 n <= 11, i32 n <= 220, i64 n <= 725,
 *            i128 n <= 725, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = n;
    seq_t n1   = nn + (seq_t)1;
    seq_t n2   = nn + (seq_t)2;
    seq_t p1   = n2 * n1;
    seq_t p2   = nn * nn + (seq_t)7 * nn - (seq_t)12;
    seq_t num  = p1 * p2;
    return num / (seq_t)24;
}
