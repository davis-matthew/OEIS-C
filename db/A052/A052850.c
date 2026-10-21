/*
 * A052850  E.g.f.: x/(1-x)+log((1-x)/(1-2*x)).
 * https://oeis.org/A052850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-1)! * (2^n + n - 1) for n > 0 a(0) = 0 (See OEIS
 *            A052850)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* factorial (n-1)! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i < n; ++i)
        fact *= (seq_t)i;

    /* compute 2^n  */
    seq_t two = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        two *= (seq_t)2;

    /* 2^n + n - 1 */
    seq_t sum = two + ((seq_t)n - (seq_t)1);

    return fact * sum;
}
