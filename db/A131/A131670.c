/*
 * A131670  Period 5: repeat [1, 0, -1, 0, 1].
 * https://oeis.org/A131670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n%5==0 or 4; -1 if n%5==2; else 0
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 5) {
    case 0:
    case 4:
        return (seq_t)1;
    case 2:
        return (seq_t)-1;
    default:
        return (seq_t)0;
    }
}
