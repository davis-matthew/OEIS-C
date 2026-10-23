/*
 * A036789  a(n) = Sum_{i=0..n} floor((2*i + 2)/(n - i + 1)).
 * https://oeis.org/A036789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n} floor((2*i+2)/(n-i+1))
 * Range:     u8 n <= 39, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t numerator   = (seq_t)i * 2 + 2;   /* 2*i + 2 */
        seq_t denominator = (seq_t)(n - i + 1); /* n - i + 1 */
        sum += numerator / denominator;        /* floor division */
    }
    return sum;
}
