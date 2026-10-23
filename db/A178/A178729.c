/*
 * A178729  a(n) = n XOR 3n, where XOR is bitwise XOR.
 * https://oeis.org/A178729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n XOR 3n
 * Range:     u8 n <= 85, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn ^ (nn * 3);
}
