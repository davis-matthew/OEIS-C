/*
 * A057174  a(n+3)=a(n)+a(n+1)-a(n+2), starting with 1,2,3.
 * https://oeis.org/A057174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2 + (n-1)*(-1)^n Simplifies to a(n) = n + 1 for even n,
 *            a(n) = 3 - n for odd n
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0)   /* even n */
        return nn + 1;
    else                /* odd n */
        return (seq_t)3 - nn;
}
