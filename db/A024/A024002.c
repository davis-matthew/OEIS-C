/*
 * A024002  a(n) = 1 - n^4.
 * https://oeis.org/A024002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n^4
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 215, i64 n <= 630,
 *            i128 n <= 630, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    return (seq_t)1 - n4;
}
