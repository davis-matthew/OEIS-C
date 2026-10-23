/*
 * A010873  a(n) = n mod 4.
 * https://oeis.org/A010873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod 4
 * Range:     u8 n <= 9302, u16 n <= 9302, u32 n <= 9302, u64 n <= 9302,
 *            u128 n <= 9302, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n) % ((seq_t)4);
}
