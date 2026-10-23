/*
 * A165750  a(n) = (10/11)*(2+9*(-10)^(n-1)).
 * https://oeis.org/A165750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-10)*a(n-1) + 20 for n ≥ 1, a(0) = 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t x = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        x = (- (seq_t)10 * x) + (seq_t)20;
    }
    return x;
}
