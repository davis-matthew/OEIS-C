/*
 * A168106  a(n) = sum of natural numbers m such that n - 7 <= m <= n + 7.
 * https://oeis.org/A168106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+7)*(n+8)/2 for 0 <= n <= 7, a(n) = 15*n for n >= 8
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 7) {
        seq_t nn = (seq_t)n;
        return (nn + 7) * (nn + 8) / 2;
    } else {
        seq_t nn = (seq_t)n;
        return 15 * nn;
    }
}
