/*
 * A033431  a(n) = 2*n^3.
 * https://oeis.org/A033431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1290, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return (tn * tn * tn) * 2;
}
