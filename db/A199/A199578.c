/*
 * A199578  Row sums of coefficient triangle of the monic associated Laguerre
 *          polynomials of order 1.
 * https://oeis.org/A199578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*n*a(n-1) - n^2*a(n-2), a(-1)=0, a(0)=1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 13, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(-1) */
    seq_t prev1 = (seq_t)1;   /* a(0) */
    seq_t cur;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t i_seq = (seq_t)i;
        cur = - (seq_t)2 * i_seq * prev1
              - i_seq * i_seq * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
