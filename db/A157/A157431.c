/*
 * A157431  a(n) = 4*n^2 + 73*n + 333.
 * https://oeis.org/A157431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 + 73*n + 333
 * Range:     u8 none, u16 n <= 118, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t res = (seq_t)4 * nn * nn + (seq_t)73 * nn + (seq_t)333;
    return res;
}
