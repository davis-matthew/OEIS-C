/*
 * A087908  Largest integer not expressible as a nonnegative linear
 *          combination of n and n^2 + 1.
 * https://oeis.org/A087908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^3 - n^2 - 1
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1290, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    seq_t n2 = nt * nt;
    seq_t n3 = n2 * nt;
    return n3 - n2 - 1;
}
