/*
 * A154590  a(n) = 2*n^2 + 16*n + 6.
 * https://oeis.org/A154590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*n^2 + 16*n + 6
 * Range:     u8 n <= 7, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 2 * (seq_t)n * (seq_t)n + 16 * (seq_t)n + 6;
    return res;
}
