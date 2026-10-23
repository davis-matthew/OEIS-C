/*
 * A178733  a(n) = n XOR 7n, where XOR is bitwise XOR.
 * https://oeis.org/A178733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n XOR 7n
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn ^ (sn * 7);
}
