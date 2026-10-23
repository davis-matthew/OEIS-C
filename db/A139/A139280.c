/*
 * A139280  a(n) = 90*n - 81.
 * https://oeis.org/A139280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 90*n - 81
 * Range:     u8 n <= 3, u16 n <= 729, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)90 - (seq_t)81;
}
