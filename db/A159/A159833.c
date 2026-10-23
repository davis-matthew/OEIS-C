/*
 * A159833  a(n) = n^2*(n^2 + 15)/4.
 * https://oeis.org/A159833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2 + 15)/4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 350, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t n2  = n_t * n_t;
    seq_t prod = n2 * (n2 + (seq_t)15);
    return prod / (seq_t)4;
}
