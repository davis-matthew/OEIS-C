/*
 * A139613  a(n) = 78*n + 13.
 * https://oeis.org/A139613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 78*n + 13
 * Range:     u8 n <= 3, u16 n <= 840, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)78 * (seq_t)n + (seq_t)13;
}
