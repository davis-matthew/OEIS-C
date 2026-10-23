/*
 * A165758  a(n) = (12-7*6^n)/5.
 * https://oeis.org/A165758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 6*a(n-1) - 12, a(0)=1
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 24, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t v = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        v = v * ((seq_t)6) - ((seq_t)12);

    return v;
}
