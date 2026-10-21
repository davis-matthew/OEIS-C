/*
 * A057428  Sign(-n): a(n) = 1 if -n > 0, = -1 if -n < 0, = 0 if n = 0.
 * https://oeis.org/A057428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 0 if n == 0, -1 otherwise
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    return (seq_t)-1;
}
