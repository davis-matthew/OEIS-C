/*
 * A168109  a(n) = sum of natural numbers m such that n - 10 <= m <= n + 10.
 * https://oeis.org/A168109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+10)*(n+11)/2 for n < 10; otherwise a(n)=21*n
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10) {
        seq_t nn = (seq_t)n;
        return (nn + 10) * (nn + 11) / 2;
    } else {
        return (seq_t)n * 21;
    }
}
