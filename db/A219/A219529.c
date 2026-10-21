/*
 * A219529  Coordination sequence for 3.3.4.3.4 Archimedean tiling.
 * https://oeis.org/A219529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; for n>0, a(n) = floor((16*n + 1) / 3)
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    return ((seq_t)16 * (seq_t)n + 1) / 3;
}
