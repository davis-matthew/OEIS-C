/*
 * A015246  a(n) = (2*n - 13)*n^2.
 * https://oeis.org/A015246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n - 13)*n^2
 * Range:     i8 n <= 7, i16 n <= 27, i32 n <= 770, i64 n <= 770,
 *            i128 n <= 770, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = (seq_t)2 * nn - (seq_t)13;
    return t * nn * nn;
}
