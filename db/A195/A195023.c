/*
 * A195023  a(n) = 14*n^2 - 4*n.
 * https://oeis.org/A195023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*n^2 - 4*n
 * Range:     u8 n <= 4, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t res = 14 * nn * nn;
    res -= 4 * nn;
    return res;
}
