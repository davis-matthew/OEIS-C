/*
 * A070590  a(n) = n^4 mod 47.
 * https://oeis.org/A070590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 47
 * Range:     u8 n <= 343, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nm = (seq_t)n % 47;
    seq_t sq = (nm * nm) % 47;
    return (sq * sq) % 47;
}
