/*
 * A181890  a(n) = 8*n^2 + 14*n + 5.
 * https://oeis.org/A181890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 + 14*n + 5
 * Range:     u8 n <= 4, u16 n <= 89, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type first. */
    seq_t m = (seq_t)n;
    seq_t res = 8 * m * m;
    res += 14 * m;
    res += 5;
    return res;
}
