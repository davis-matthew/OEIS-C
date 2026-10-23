/*
 * A168099  a(n) = number of natural numbers m such that n - 9 <= m <= n + 9.
 * https://oeis.org/A168099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 9 for n <= 9; a(n) = 19 for n > 9 (offset 0)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10u) {
        return (seq_t)n + 9;
    } else {
        return 19;
    }
}
