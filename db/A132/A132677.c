/*
 * A132677  Period 3: repeat [1, 2, -3].
 * https://oeis.org/A132677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n%3==0, 2 if n%3==1, -3 if n%3==2
 * Range:     i8 n <= 98, i16 n <= 98, i32 n <= 98, i64 n <= 98, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic array of values */
    static const seq_t tbl[3] = { (seq_t)1, (seq_t)2, (seq_t)-3 };
    return tbl[n % 3];
}
