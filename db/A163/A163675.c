/*
 * A163675  a(n) = n*(2*n^2 + 5*n + 19)/2.
 * https://oeis.org/A163675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 5*n + 19)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = (seq_t)2 * nn * nn + (seq_t)5 * nn + (seq_t)19;
    return nn * t / 2;
}
