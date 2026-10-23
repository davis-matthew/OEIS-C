/*
 * A010673  Period 2: repeat [0, 2].
 * https://oeis.org/A010673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - (-1)^n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) = 0 if n is even, 2 if n is odd */
    return ((seq_t)(n & 1)) * ((seq_t)2);
}
