/*
 * A158634  a(n) = 46*n^2 - 1.
 * https://oeis.org/A158634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 46*n*n - 1
 * Range:     u8 n <= 2, u16 n <= 37, u32 n <= 9662, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 46 * nn * nn - 1;
}
