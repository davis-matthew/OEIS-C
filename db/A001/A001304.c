/*
 * A001304  Expansion of 1/((1-x)^2*(1-x^2)*(1-x^5)).
 * https://oeis.org/A001304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+8)*(2*n^2+11*n+18)/120)
 * Range:     u8 n <= 8, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t poly = 2 * nn * nn + 11 * nn + 18;
    return (nn + 8) * poly / 120;
}
