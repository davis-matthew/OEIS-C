/*
 * A152032  a(n) = 3/8 + (3/8)*(-1)^n + ((n+1)/4)*(-1)^(n+1) +
 *          ((n+2)*(n+1)/4)*(-1)^(n+2).
 * https://oeis.org/A152032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (3 + (-1)^n * (2 n^2 + 4 n + 5)) / 8
 * Range:     i8 n <= 7, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t expr = (seq_t)2 * nn * nn + (seq_t)4 * nn + (seq_t)5;
    seq_t sign = ((n & 1) == 0) ? (seq_t)1 : -(seq_t)1;
    seq_t num  = (seq_t)3 + sign * expr;
    return num / (seq_t)8;
}
