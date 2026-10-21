/*
 * A087912  Exponential generating function is exp(2*x/(1-x))/(1-x).
 * https://oeis.org/A087912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)*a(n-1) - (n-1)^2 * a(n-2), a(0)=1, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)3;

    seq_t a_nm2 = (seq_t)1;          /* a(n-2), starts with a(0) */
    seq_t a_nm1 = (seq_t)3;          /* a(n-1), starts with a(1) */
    seq_t a_n   = (seq_t)0;          /* placeholder */

    for (idx_t i = 2U; i <= n; ++i)
    {
        /* coefficient (2*i+1) as seq_t */
        seq_t coeff1 = ((seq_t)i << 1) + 1;               /* 2*i + 1 */

        /* coefficient (i-1)^2 as seq_t */
        seq_t ip1   = (seq_t)(i - 1U);                    /* i-1 */
        seq_t coeff2 = ip1 * ip1;                         /* (i-1)^2 */

        /* recurrence: a(i) = coeff1 * a_nm1 - coeff2 * a_nm2 */
        a_n = coeff1 * a_nm1 - coeff2 * a_nm2;

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
