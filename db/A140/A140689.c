/*
 * A140689  a(n) = n*(3*n + 20).
 * https://oeis.org/A140689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(3*n+20)
 * Range:     u8 n <= 6, u16 n <= 144, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn * 3 + (seq_t)20);
}
