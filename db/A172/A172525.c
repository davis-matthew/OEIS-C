/*
 * A172525  a(n) = 111111111 * n.
 * https://oeis.org/A172525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 111111111 * n
 * Range:     u8 none, u16 none, u32 n <= 38, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)111111111ULL;
}
