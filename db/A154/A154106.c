/*
 * A154106  a(n) = 12*n^2 + 22*n + 11.
 * https://oeis.org/A154106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^2 + 22*n + 11
 * Range:     u8 n <= 3, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 12 * nn * nn + 22 * nn + 11;
    return res;
}
