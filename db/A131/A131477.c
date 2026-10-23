/*
 * A131477  a(n) = ceiling(n^3/3).
 * https://oeis.org/A131477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^3/3)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1786, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t n2 = n_t * n_t;
    seq_t n3 = n2 * n_t;
    return (n3 + (seq_t)2) / (seq_t)3;
}
