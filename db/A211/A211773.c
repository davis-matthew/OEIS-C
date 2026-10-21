/*
 * A211773  Prime-generating polynomial: a(n) = 2*n^2 - 108*n + 1259.
 * https://oeis.org/A211773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n^2 - 108*n + 1259
 * Range:     i8 none, i16 n <= 155, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 2 * nn * nn - 108 * nn + 1259;
}
