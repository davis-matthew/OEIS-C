/*
 * A052591  Expansion of e.g.f. x/((1-x)(1-x^2)).
 * https://oeis.org/A052591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)!/2 if n is odd; n!*n/2 if n is even
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t mul;
    if (n & 1)           /* odd n: (n+1)/2 */
        mul = (seq_t)((n + 1) / 2);
    else                 /* even n: n/2 */
        mul = (seq_t)(n / 2);

    return fact * mul;
}
