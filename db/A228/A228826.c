/*
 * A228826  Delayed continued fraction of sqrt(2).
 * https://oeis.org/A228826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, sign, easy
 * Formula:   a(n) = {2,-1,-2,1}[n mod 4]; equivalently a(0)=2, a(1)=-1,
 *            a(n)=-a(n-2)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[4] = { (seq_t) 2, (seq_t)-1, (seq_t)-2, (seq_t) 1 };
    return tbl[n & 3];
}
