/*
 * A052649  Expansion of e.g.f. (2+x-x^2)/(1-x)^2.
 * https://oeis.org/A052649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3 + 2*n) * n! , with a(0) = 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;              /* compute n! */

    return fact * (seq_t)(3 + 2 * n);  /* multiply by 3 + 2*n */
}
