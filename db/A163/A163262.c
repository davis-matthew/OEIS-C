/*
 * A163262  Denominators of fractions in the approximation of the square root
 *          of 2 by means of: f(n) = 3*f(n-1)/(f(n-1)^2+1); with f(1)= 1.
 * https://oeis.org/A163262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(1)=1; for n>1 let f(1)=1 and f(n)=3*f(n-1)/(f(n-1)^2+1); a(n)
 *            is the denominator of f(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t p = 1;
    seq_t q = 1;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t new_p = (p * q) * 3;     /* 3 * p * q */
        seq_t new_q = p * p + q * q;  /* p^2 + q^2 */
        p = new_p;
        q = new_q;
    }

    return q;
}
