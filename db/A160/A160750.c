/*
 * A160750  Expansion of (1+11*x+24*x^2+11*x^3+10*x^4)/(1-x)^5.
 * https://oeis.org/A160750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (19*n^4 + 14*n^3 + 77*n^2 + 10*n + 8)/8
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 204, u64 n <= 10000,
 *            u128 n <= 10000, big any n
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
    seq_t num = 19 * n4 + 14 * n3 + 77 * n2 + 10 * nn + 8;
    return num / 8;
}
