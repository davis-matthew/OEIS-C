/*
 * A085687  Expansion of g.f. 8/(1+sqrt(1-8*x))^3.
 * https://oeis.org/A085687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*(5*n+7)*a(n-1) - 8*(2*n-1)*a(n-2)) / (n+3) with
 *            initial values a(0) = 1, a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)6;

    seq_t a_nm2 = (seq_t)1;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)6;  /* a(n-1) */
    seq_t a_n   = (seq_t)0;  /* a(n) */

    for (idx_t i = 2u; i <= n; ++i)
    {
        /* compute numerator = 2*(5*i+7)*a_nm1 - 8*(2*i-1)*a_nm2 */
        seq_t term1 = (seq_t)2 * (seq_t)(5u * i + 7u) * a_nm1;
        seq_t term2 = (seq_t)8 * (seq_t)(2u * i - 1u) * a_nm2;
        seq_t numerator = term1 - term2;

        seq_t denom = (seq_t)(i + 3u);
        a_n = numerator / denom;   /* exact division */

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
