/*
 * A140678  a(n) = n*(3*n + 10).
 * https://oeis.org/A140678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + 10*n
 * Range:     u8 n <= 7, u16 n <= 146, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (3 * nn + 10);
}
