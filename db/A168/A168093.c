/*
 * A168093  a(n) = number of natural numbers m such that n - 3 <= m <= n + 3.
 * https://oeis.org/A168093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 3 + n for 0 <= n <= 3, a(n) = 7 for n >= 4
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)(3 + n);
    return (seq_t)7;
}
