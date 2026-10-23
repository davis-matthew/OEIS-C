/*
 * A132411  a(0) = 0, a(1) = 1 and a(n) = n^2 - 1 with n >= 2.
 * https://oeis.org/A132411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = n*n - 1 for n >= 2
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t nn = (seq_t)n;
    return nn * nn - (seq_t)1;
}
