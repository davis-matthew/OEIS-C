/*
 * A058373  a(n) = (1/6)*(2*n - 3)*(n + 2)*(n + 1).
 * https://oeis.org/A058373
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/6)*(2*n - 3)*(n + 2)*(n + 1)
 * Range:     i8 n <= 3, i16 n <= 24, i32 n <= 1769, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = ((((seq_t)2 * nn) - (seq_t)3) * (nn + (seq_t)2)) * (nn + (seq_t)1);
    val = val / (seq_t)6;
    return val;
}
