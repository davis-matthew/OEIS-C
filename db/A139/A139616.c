/*
 * A139616  a(n) = 120*n + 16.
 * https://oeis.org/A139616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 120*n + 16
 * Range:     u8 n <= 1, u16 n <= 545, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)120 * n + 16;
}
