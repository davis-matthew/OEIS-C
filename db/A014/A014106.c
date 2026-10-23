/*
 * A014106  a(n) = n*(2*n + 3).
 * https://oeis.org/A014106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n*(2*n + 3)
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 920, u64 n <= 920,
 *            u128 n <= 920, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn * 2 + 3);
}
