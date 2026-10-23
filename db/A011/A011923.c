/*
 * A011923  a(n) = floor(n*(n-1)*(n-2)*(n-3)/13).
 * https://oeis.org/A011923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/13)
 * Range:     u8 n <= 9, u16 n <= 31, u32 n <= 486, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    seq_t n0 = (seq_t)n;
    seq_t n1 = n0 - 1;
    seq_t n2 = n0 - 2;
    seq_t n3 = n0 - 3;
    return (n0 * n1 * n2 * n3) / 13;
}
