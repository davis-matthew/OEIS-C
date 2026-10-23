/*
 * A015536  Expansion of x/(1-5*x-3*x^2).
 * https://oeis.org/A015536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1; a(n) = 5*a(n-1) + 3*a(n-2) for n > 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a_nm2 = (seq_t)0; /* a(0) */
    seq_t a_nm1 = (seq_t)1; /* a(1) */
    seq_t a_n   = a_nm1;    /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        a_n = (seq_t)5 * a_nm1 + (seq_t)3 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
