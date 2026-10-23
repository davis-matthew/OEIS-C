/*
 * A139606  a(n) = 15*n + 6.
 * https://oeis.org/A139606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 15*n + 6
 * Range:     u8 n <= 16, u16 n <= 4368, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;
    result = result * (seq_t)15 + (seq_t)6;
    return result;
}
