/*
 * A141759  a(n) = 16n^2 + 32n + 15.
 * https://oeis.org/A141759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 + 32*n + 15
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return (seq_t)16 * x * x + (seq_t)32 * x + (seq_t)15;
}
