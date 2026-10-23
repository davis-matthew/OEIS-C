/*
 * A192759  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x in the polynomial p(n,x) where p(0,x) =
 *            1, p(n,x) = x * p(n‑1,x) + floor((n+5)/5). Reduction uses the
 *            rule x² = x + 1. The coefficients of p(n,x) are always of the
 *            form c0 + c1*x; a(n) = c1
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t c0 = (seq_t)1;    /* constant term */
    seq_t c1 = (seq_t)0;    /* coefficient of x */
    if (n == 0) return (seq_t)0;   /* a(0) = 0 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* integer floor of (i+5)/5 */
        seq_t add = (seq_t)((i + 5) / 5);
        seq_t new_c1 = c0 + c1;      /* (p * x) coefficient of x   */
        seq_t new_c0 = c1 + add;     /* constant term after multiplication and addition */
        c0 = new_c0;
        c1 = new_c1;
    }
    return c1;   /* coefficient of x in p(n,x) */
}
