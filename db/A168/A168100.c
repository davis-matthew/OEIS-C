/*
 * A168100  a(n) = number of natural numbers m such that n - 10 <= m <= n +
 *          10.
 * https://oeis.org/A168100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { n + 10, for 0 <= n <= 10; 21, for n >= 11 }
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 10) {
        return (seq_t)n + (seq_t)10;
    } else {
        return (seq_t)21;
    }
}
