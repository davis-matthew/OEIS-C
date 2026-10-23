/*
 * A146302  a(n) = (8*n+5)*(8*n+9).
 * https://oeis.org/A146302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+5)*(8*n+9)
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn * 8 + 5) * (nn * 8 + 9);
}
