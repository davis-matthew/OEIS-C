/*
 * A159928  a(n) is the sum of the terms of row n of triangle A159927.
 * https://oeis.org/A159928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)*a(n-1) - n^2*a(n-2) + n*a(n-3) \ * with initial
 *            values a(1)=1, a(2)=3, a(3)=15
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined for n<1, return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)15;

    /* a(n-3), a(n-2), a(n-1) for current k */
    seq_t a_nm3 = (seq_t)1;  /* a(1) */
    seq_t a_nm2 = (seq_t)3;  /* a(2) */
    seq_t a_nm1 = (seq_t)15; /* a(3) */
    seq_t a_cur = 0;

    for (idx_t k = 4; k <= n; ++k)
    {
        seq_t term1 = (seq_t)(2 * k + 1) * a_nm1;  /* (2k+1)*a(n-1) */
        seq_t term2 = (seq_t)(k * k)      * a_nm2;  /* k^2    *a(n-2) */
        seq_t term3 = (seq_t)k            * a_nm3;  /* k      *a(n-3) */

        a_cur = term1 - term2 + term3; /* recurrence */

        /* shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }

    return a_cur;
}
