/*
 * A074085  Coefficient of q^2 in nu(n), where nu(0) = 1, nu(1) = b and, for n
 *          >= 2, nu(n) = b*nu(n-1) + lambda*(1 + q + q^2 + ... + q^(n -
 *          2))*nu(n-2) with (b,lambda) = (2,1).
 * https://oeis.org/A074085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of q^2 in nu(n) where * nu(0)=1, nu(1)=2, *
 *            nu(n)=2*nu(n-1)+(1+q+...+q^(n-2))*nu(n-2) (for n≥2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* coefficients for degree 0,1,2 */
    seq_t c0, b0, a0; /* for n-2 */
    seq_t c1, b1, a1; /* for n-1 */
    seq_t c,  b,  a;  /* for current n */

    /* base: n=0,1 */
    c0 = (seq_t)1;  b0 = 0; a0 = 0;   /* nu(0) = 1 */
    c1 = (seq_t)2;  b1 = 0; a1 = 0;   /* nu(1) = 2 */

    if (n==0 || n==1) return (seq_t)0;   /* coefficient of q^2 is 0 */

    for (idx_t i=2; i<=n; ++i)
    {
        /* coefficients of S_i = 1 + q + ... + q^(i-2) up to degree 2 */
        seq_t s0 = 1;
        seq_t s1 = (i>=3) ? 1 : 0;
        seq_t s2 = (i>=4) ? 1 : 0;

        /* convolution S_i * nu(i-2) */
        seq_t const_term = s0 * c0;
        seq_t x1_term   = s0 * b0 + s1 * c0;
        seq_t x2_term   = s0 * a0 + s1 * b0 + s2 * c0;

        /* 2 * nu(i-1) */
        seq_t double_c = c1 + c1;
        seq_t double_b = b1 + b1;
        seq_t double_a = a1 + a1;

        /* resulting coefficients */
        c = double_c + const_term;
        b = double_b + x1_term;
        a = double_a + x2_term;

        /* shift for next iteration */
        c0 = c1;  b0 = b1;  a0 = a1;
        c1 = c;   b1 = b;   a1 = a;
    }

    /* a holds coefficient of q^2 for nu(n) */
    return a;
}
