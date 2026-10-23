/*
 * A122368  Dimension of 4-variable non-commutative harmonics (twisted
 *          derivative). The dimension of the space of non-commutative
 *          polynomials in 4 variables which are killed by all symmetric
 *          differential operators (where for a monomial w, d_{xi} ( xi w ) =
 *          w and d_{xi} ( xj w ) = 0 for i/=j).
 * https://oeis.org/A122368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1)=1, a(2)=3, a(3)=11; a(n)=6*a(n-1)-9*a(n-2)+3*a(n-3) for n>3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1, undefined for n=0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)11;

    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)3;   /* a(2) */
    seq_t a3 = (seq_t)11;  /* a(3) */
    for (idx_t k = 4; k <= n; ++k)
    {
        seq_t val = (seq_t)6 * a3 - (seq_t)9 * a2 + (seq_t)3 * a1;
        a1 = a2;
        a2 = a3;
        a3 = val;
    }
    return a3;
}
