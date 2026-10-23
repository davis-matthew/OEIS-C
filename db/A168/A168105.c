/*
 * A168105  a(n) = sum of natural numbers m such that n - 6 <= m <= n + 6.
 * https://oeis.org/A168105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+6)*(n+7)/2 for 0 <= n <= 6, a(n) = 13*n for n >= 7
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 6)
    {
        seq_t x = (seq_t)n + 6;
        seq_t y = (seq_t)n + 7;
        return x * y / 2;
    }
    else
    {
        return (seq_t)13 * (seq_t)n;
    }
}
