/*
 * A227582  Expansion of
 *          (2+3*x+2*x^2+2*x^3+3*x^4+x^5-x^6)/(1-2*x+x^2-x^5+2*x^6-x^7).
 * https://oeis.org/A227582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((6*n^2+6*n-1)/5)
 * Range:     u8 n <= 6, u16 n <= 144, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t num = 6*nn*nn + 6*nn - 1;
    return num / 5;
}
