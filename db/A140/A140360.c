/*
 * A140360  Inverse binomial transform of A140359.
 * https://oeis.org/A140360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1 a(1) = 0 a(2) = 5 a(n) = 5 - 2*a(n-1) for n > 2 (linear
 *            recurrence)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return 5;

    seq_t val = 5;          /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        val = 5 - (val * 2);
    }
    return val;
}
