/*
 * A168097  a(n) = number of natural numbers m such that n - 7 <= m <= n + 7.
 * https://oeis.org/A168097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = n+7 for 0 <= n <= 8, 15 for n >= 9
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 8)
        return (seq_t)n + 7;
    else
        return 15;
}
