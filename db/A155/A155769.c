/*
 * A155769  a(n) = 2*n^2 + 2*n - 41.
 * https://oeis.org/A155769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n^2 + 2*n - 41
 * Range:     i8 n <= 8, i16 n <= 127, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn * 2;
    res += nn * 2;
    res -= (seq_t)41;
    return res;
}
