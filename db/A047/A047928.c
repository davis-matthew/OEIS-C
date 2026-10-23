/*
 * A047928  a(n) = n*(n-1)^2*(n-2).
 * https://oeis.org/A047928
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)^2*(n-2)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t n1  = (seq_t)(n - 1);
    seq_t n2  = (seq_t)(n - 2);
    return n_t * n1 * n1 * n2;
}
