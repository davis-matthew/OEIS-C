/*
 * A213370  a(n) = n AND 2*n, where AND is the bitwise AND operator.
 * https://oeis.org/A213370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n & 2*n (bitwise AND)
 * Range:     u8 n <= 383, u16 n <= 9576, u32 n <= 9576, u64 n <= 9576,
 *            u128 n <= 9576, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n) & (((seq_t)n) << 1);
}
