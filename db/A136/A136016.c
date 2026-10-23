/*
 * A136016  a(n) = 9*n^2-1.
 * https://oeis.org/A136016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2 - 1
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * (seq_t)n;
    return ((seq_t)9 * nn) - (seq_t)1;
}
