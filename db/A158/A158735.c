/*
 * A158735  a(n) = 1225*n^2 - 35.
 * https://oeis.org/A158735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1225*n^2 - 35
 * Range:     u8 none, u16 n <= 7, u32 n <= 1872, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = (seq_t)1225 * nn * nn - (seq_t)35;
    return res;
}
