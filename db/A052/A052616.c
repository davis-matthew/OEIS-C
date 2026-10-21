/*
 * A052616  Expansion of e.g.f. (3+2*x)/(1-x^2).
 * https://oeis.org/A052616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3 * n! if n is even; 2 * n! otherwise
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
    }
    seq_t coeff = (n & 1) ? 2 : 3;
    return fact * coeff;
}
