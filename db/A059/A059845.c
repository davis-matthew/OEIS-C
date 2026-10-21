/*
 * A059845  a(n) = n*(3*n + 11)/2.
 * https://oeis.org/A059845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(3*n + 11)/2
 * Range:     u8 n <= 11, u16 n <= 207, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t tmp = (seq_t)3 * nn + (seq_t)11;
    seq_t res = nn * tmp / (seq_t)2;
    return res;
}
