/*
 * A139608  a(n) = 28*n + 8.
 * https://oeis.org/A139608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 28*n + 8
 * Range:     u8 n <= 8, u16 n <= 2340, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n) * 28 + 8;
}
