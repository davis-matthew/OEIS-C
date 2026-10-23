/*
 * A129936  a(n) = (n-2)*(n+3)*(n+2)/6.
 * https://oeis.org/A129936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (n-2)*(n+3)*(n+2)/6
 * Range:     i8 n <= 4, i16 n <= 31, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = (nn - (seq_t)2) * (nn + (seq_t)3) * (nn + (seq_t)2);
    res /= (seq_t)6;
    return res;
}
