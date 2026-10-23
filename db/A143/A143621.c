/*
 * A143621  a(n) = (-1)^binomial(n,4): Periodic sequence
 *          1,1,1,1,-1,-1,-1,-1,... .
 * https://oeis.org/A143621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^floor(n/4) (period 8, 1 for n%8=0..3, -1 for 4..7)
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n & 7;               /* n % 8, faster using bitwise AND for powers of 2 */
    return (m < 4) ? (seq_t)1 : (seq_t)-1;
}
