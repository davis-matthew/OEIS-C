/*
 * A022282  a(n) = n*(25*n - 1)/2.
 * https://oeis.org/A022282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(25*n - 1)/2
 * Range:     u8 n <= 4, u16 n <= 72, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = nn * (nn * 25 - 1) / 2;
    return result;
}
