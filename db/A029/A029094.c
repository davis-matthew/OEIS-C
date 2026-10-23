/*
 * A029094  Expansion of 1/((1-x)*(1-x^5)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A029094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((83*n^3+84*n^2+43*n+66)/66) -
 *            floor((2*n^3+2*n^2+n+1)/5) - floor((6*n^3+6*n^2+2*n+2)/7)
 * Range:     u8 n <= 11, u16 n <= 40, u32 n <= 387, u64 n <= 1000,
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

    seq_t t1 = (83 * n3 + 84 * n2 + 43 * nn + 66) / 66;
    seq_t t2 = (2 * n3 + 2 * n2 + nn + 1) / 5;
    seq_t t3 = (6 * n3 + 6 * n2 + 2 * nn + 2) / 7;

    return t1 - t2 - t3;
}
