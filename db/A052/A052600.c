/*
 * A052600  Expansion of e.g.f. 1/((1-2*x)*(1-x^2)).
 * https://oeis.org/A052600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * ((4*2^n - (n%2==0?1:2))/3) (OEIS A052600)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
        pow2 <<= 1;            /* multiply by 2 */
    }

    /* coefficient (4*2^n - (n even ? 1 : 2)) / 3 */
    seq_t coeff = 4 * pow2;
    coeff -= (n & 1) ? 2 : 1;
    coeff /= 3;

    /* result */
    return fact * coeff;
}
