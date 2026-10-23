/*
 * A160767  Expansion of (1+12*x+28*x^2+12*x^3+x^4)/(1-x)^5.
 * https://oeis.org/A160767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^4/4 + 9*n^3/2 + 23*n^2/4 + 7*n/2 + 1
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 181, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t num = 9 * n4 + 18 * n3 + 23 * n2 + 14 * nn + 4;
    return num / 4;
}
