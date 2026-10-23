/*
 * A113532  a(n) = 1 + 2*n + 3*n^2 + 4*n^3 + 5*n^4 + 6*n^5 + 7*n^6.
 * https://oeis.org/A113532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 2*n + 3*n^2 + 4*n^3 + 5*n^4 + 6*n^5 + 7*n^6
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 7;
    res = res * nn + 6;
    res = res * nn + 5;
    res = res * nn + 4;
    res = res * nn + 3;
    res = res * nn + 2;
    res = res * nn + 1;
    return res;
}
