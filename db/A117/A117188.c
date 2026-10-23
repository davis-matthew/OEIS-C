/*
 * A117188  Expansion of (1-x^2)/(1+x^2+x^4).
 * https://oeis.org/A117188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-6) for n>5; a(0)=1, a(1)=0, a(2)=-2, a(3)=0, a(4)=1,
 *            a(5)=0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[6] = {1, 0, -2, 0, 1, 0};
    return tbl[(int)(n % 6)];
}
