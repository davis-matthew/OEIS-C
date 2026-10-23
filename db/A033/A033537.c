/*
 * A033537  a(n) = n*(2*n+5).
 * https://oeis.org/A033537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n+5)
 * Range:     u8 n <= 10, u16 n <= 179, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn * 2 + 5);
}
