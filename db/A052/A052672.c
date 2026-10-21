/*
 * A052672  Expansion of e.g.f. (1-x)/(1-x-2*x^2+x^3).
 * https://oeis.org/A052672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*a(n-1) + 2*n*(n-1)*a(n-2) - n*(n-1)*(n-2)*a(n-3)
 *            Initial values: a(0)=1, a(1)=0, a(2)=4
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;

    /* Sliding window for recurrence: a(n-3), a(n-2), a(n-1) */
    seq_t a_nm3 = (seq_t)1;
    seq_t a_nm2 = (seq_t)0;
    seq_t a_nm1 = (seq_t)4;
    seq_t a_n   = a_nm1; /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        /* term1 = i * a(n-1) */
        seq_t term1 = (seq_t)i * a_nm1;

        /* term2 = 2 * i * (i-1) * a(n-2) */
        seq_t coef2 = (seq_t)2 * i * (i - 1);   /* safe: i<=10000 */
        seq_t term2 = coef2 * a_nm2;

        /* term3 = i * (i-1) * (i-2) * a(n-3) */
        seq_t coef3 = (seq_t)i * (i - 1) * (i - 2);
        seq_t term3 = coef3 * a_nm3;

        /* a(n) = term1 + term2 - term3; safe because term3 <= term1+term2 */
        a_n = term1 + term2;
        a_n -= term3;

        /* shift window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
