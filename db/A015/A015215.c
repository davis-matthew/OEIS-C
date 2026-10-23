/*
 * A015215  Sum of Gaussian binomial coefficients for q=23.
 * https://oeis.org/A015215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2, a(n) = 2*a(n-1) + a(n-2)*((23^(n-1)) - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)2;

    seq_t a_nm2 = (seq_t)1;          /* a(0) */
    seq_t a_nm1 = (seq_t)2;          /* a(1) */
    seq_t pow23 = (seq_t)1;          /* 23^0 */

    for (idx_t k = (idx_t)2; k <= n; ++k)
    {
        pow23 = pow23 * (seq_t)23;           /* pow23 = 23^(k-1) */
        seq_t temp   = pow23 - (seq_t)1;     /* 23^(k-1) - 1 */
        seq_t term   = a_nm2 * temp;         /* a(n-2) * (23^(k-1) - 1) */
        seq_t a_n    = a_nm1 * (seq_t)2 + term; /* 2*a(n-1) + term */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
