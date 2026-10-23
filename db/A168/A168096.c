/*
 * A168096  a(n) = number of natural numbers m such that n - 6 <= m <= n + 6.
 * https://oeis.org/A168096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 6 + n for 0 <= n <= 6, a(n) = 13 for n >= 7
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 6)
        return (seq_t)n + 6;
    else
        return 13;
}
