/*
 * A165759  a(n) = (7-4*7^n)/3.
 * https://oeis.org/A165759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 7*a(n-1) - 14, a(0) = 1
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 22, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        res = 7 * res - 14;
    }
    return res;
}
