/*
 * A135403  a(n) = 1 + 111110*n.
 * https://oeis.org/A135403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 111110*n
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n * 111110) + 1;
}
