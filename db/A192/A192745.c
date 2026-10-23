/*
 * A192745  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) - (n-2)*a(n-2) - (n-1)*a(n-3), with a(0)=0,
 *            a(1)=1, a(2)=2. This recurrence follows from the OEIS
 *            description
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    seq_t a_nm3 = 0;   /* a(n-3) */
    seq_t a_nm2 = 1;   /* a(n-2) */
    seq_t a_nm1 = 2;   /* a(n-1) */
    seq_t a_n   = 0;   /* a(n)   */

    for (idx_t i = 3; i <= n; ++i)
    {
        a_n = (seq_t)i * a_nm1
            - (seq_t)(i - 2) * a_nm2
            - (seq_t)(i - 1) * a_nm3;

        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
