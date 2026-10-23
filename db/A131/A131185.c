/*
 * A131185  Period 6: repeat [0, 2, -1, 0, -1, 3].
 * https://oeis.org/A131185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-6) for n>5; a(0)=0, a(1)=2, a(2)=-1, a(3)=0, a(4)=-1,
 *            a(5)=3
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* table of one full period */
    static const seq_t period[6] = {0, 2, -1, 0, -1, 3};
    return period[n % 6];
}
