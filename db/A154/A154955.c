/*
 * A154955  a(1) = 1, a(2) = -1, followed by 0, 0, 0, ... .
 * https://oeis.org/A154955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, sign, easy
 * Formula:   a(n) = [n = 1] - [n = 2] (Iverson brackets)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-1;
    return (seq_t)0;
}
