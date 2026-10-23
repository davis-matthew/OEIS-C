/*
 * A022285  a(n) = n*(27*n + 1)/2.
 * https://oeis.org/A022285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (27 * n + 1) / 2
 * Range:     u8 n <= 4, u16 n <= 69, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn * (seq_t)27 + 1) / 2;
}
