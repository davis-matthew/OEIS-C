/*
 * A168101  a(n) = sum of natural numbers m such that n - 2 <= m <= n + 2.
 * https://oeis.org/A168101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n for n >= 2, with special initial values a(0)=3,
 *            a(1)=6
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)6;
    return (seq_t)n * (seq_t)5;
}
