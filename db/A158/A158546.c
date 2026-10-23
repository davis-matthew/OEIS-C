/*
 * A158546  a(n) = 144*n^2 + 12.
 * https://oeis.org/A158546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 144*n^2 + 12
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 5461, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)144 * nn * nn + (seq_t)12;
}
