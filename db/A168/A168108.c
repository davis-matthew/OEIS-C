/*
 * A168108  a(n) = sum of natural numbers m such that n - 9 <= m <= n + 9.
 * https://oeis.org/A168108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+9)*(n+10)/2 for 0 ≤ n ≤ 9, a(n) = 19*n for n ≥ 10
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 9)
    {
        seq_t nn = (seq_t)n;
        return (nn + (seq_t)9) * (nn + (seq_t)10) / (seq_t)2;
    }
    return 19 * (seq_t)n;
}
