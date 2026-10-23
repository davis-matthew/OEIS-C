/*
 * A139615  a(n) = 105*n + 15.
 * https://oeis.org/A139615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 105*n + 15
 * Range:     u8 n <= 2, u16 n <= 624, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)105 * (seq_t)n + (seq_t)15;
}
