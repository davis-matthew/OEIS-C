/*
 * A061722  a(n) = 10*n^2 + 7.
 * https://oeis.org/A061722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*n^2 + 7
 * Range:     u8 n <= 4, u16 n <= 80, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x * x * (seq_t)10 + (seq_t)7;
}
