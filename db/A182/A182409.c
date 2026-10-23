/*
 * A182409  Prime-generating polynomial: a(n) = 4*n^2 + 12*n - 1583.
 * https://oeis.org/A182409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*n^2 + 12*n - 1583
 * Range:     i8 none, i16 n <= 91, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 4 * nn * nn + 12 * nn - (seq_t)1583;
}
