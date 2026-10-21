/*
 * A089594  Alternating sum of squares to n.
 * https://oeis.org/A089594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * n*(n+1)/2
 * Range:     i8 n <= 15, i16 n <= 255, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nSeq = (seq_t)n;
    seq_t tri  = nSeq * (nSeq + 1) / 2;
    return (n & 1) ? -tri : tri;
}
