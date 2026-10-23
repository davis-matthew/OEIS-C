/*
 * A152618  a(n) = (n-1)^2*(n+1).
 * https://oeis.org/A152618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - n^2 - n + 1 (equivalent to (n-1)^2 * (n+1))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t n2 = n_t * n_t;
    seq_t n3 = n2 * n_t;
    return n3 - n2 - n_t + 1;
}
