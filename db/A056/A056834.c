/*
 * A056834  a(n) = floor(n^2/7).
 * https://oeis.org/A056834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2/7)
 * Range:     u8 n <= 42, u16 n <= 677, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)(((idx_t)n * (idx_t)n) / 7);
}
