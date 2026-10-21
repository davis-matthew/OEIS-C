/*
 * A219779  1 = Sum_{n>=0} a(n) * x^n * (1 - (n+1)*x)^3.
 * https://oeis.org/A219779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n*a(n-1) - 3*(n-1)^2*a(n-2) + (n-2)^3*a(n-3), with
 *            a(0)=1, a(1)=3, a(2)=15
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)15;

    seq_t a_nm3 = (seq_t)1;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)3;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)15;  /* a(n-1)  */
    seq_t a_n;                /* a(n)    */

    for (idx_t i = 3; i <= n; ++i) {
        /* term1 = 3 * i * a(n-1) */
        seq_t term1 = 3 * (seq_t)i * a_nm1;

        /* term2 = 3 * (i-1)^2 * a(n-2) */
        seq_t t = (seq_t)(i - 1);
        seq_t t2 = t * t;                 /* (i-1)^2 */
        seq_t term2 = 3 * t2 * a_nm2;

        /* term3 = (i-2)^3 * a(n-3) */
        seq_t s = (seq_t)(i - 2);
        seq_t s3 = s * s * s;             /* (i-2)^3 */
        seq_t term3 = s3 * a_nm3;

        /* a(n) = term1 - term2 + term3   (modular arithmetic on seq_t) */
        a_n = term1 - term2 + term3;

        /* shift the window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
