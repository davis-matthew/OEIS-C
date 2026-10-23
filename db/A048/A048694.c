/*
 * A048694  Generalized Pellian with second term equal to 7.
 * https://oeis.org/A048694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2); a(0) = 1, a(1) = 7;
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t a_nm2 = (seq_t)1;            /* a(n-2) */
    seq_t a_nm1 = (seq_t)7;            /* a(n-1) */
    seq_t a_n;                         /* current term */

    for (idx_t i = 2; i <= n; ++i)
    {
        a_n = a_nm1 * (seq_t)2 + a_nm2;  /* a(n) = 2*a(n-1) + a(n-2) */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
