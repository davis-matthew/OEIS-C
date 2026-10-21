/*
 * A053128  Binomial coefficients C(2*n-5,6).
 * https://oeis.org/A053128
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-5, 6) =
 *            (n-5)*(n-4)*(n-3)*(2n-9)*(2n-7)*(2n-5)/90
 * Range:     u8 none, u16 n <= 8, u32 n <= 32, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6) return 0;
    seq_t tn = (seq_t)n;
    seq_t res = (tn - 5);
    res *= (tn - 4);
    res *= (tn - 3);
    res *= (2 * tn - 9);
    res *= (2 * tn - 7);
    res *= (2 * tn - 5);
    res /= 90;
    return res;
}
