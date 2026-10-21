/*
 * A057079  Periodic sequence: repeat [1,2,1,-1,-2,-1]; expansion of
 *          (1+x)/(1-x+x^2).
 * https://oeis.org/A057079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1, 2, 1, -1, -2, -1 repeated (period 6)
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[6] = { 1, 2, 1, -1, -2, -1 };
    return tbl[n % 6];
}
