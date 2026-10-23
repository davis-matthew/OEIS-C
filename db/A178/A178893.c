/*
 * A178893  a(n) = n OR 6n, where OR is bitwise OR.
 * https://oeis.org/A178893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n | (6 * n)
 * Range:     u8 n <= 42, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n) | (((seq_t)n) * 6);
}
