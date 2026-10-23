/*
 * A157948  a(n) = 64*n^2 - n.
 * https://oeis.org/A157948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*n*n - n
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 8192, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 64 * nn * nn - nn;
}
