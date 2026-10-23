/*
 * A143622  a(n) = (-1)^binomial(n,8): Periodic sequence
 *          1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,... .
 * https://oeis.org/A143622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^floor(n/8) (period 16: 8 ones then 8 minus ones)
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/8) parity determines sign */
    return ((n >> 3) & 1) == 0 ? 1 : -1;
}
