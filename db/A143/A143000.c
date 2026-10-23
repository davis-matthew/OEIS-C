/*
 * A143000  a(0) = 0, a(1) = 1, a(n+1) = 3*(2*n+1)*a(n) + n^4*a(n-1).
 * https://oeis.org/A143000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1, a(n+1)=3*(2*n+1)*a(n)+n^4*a(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t a_nm2 = (seq_t)0;   /* a(n-2) initially a0 */
    seq_t a_nm1 = (seq_t)1;   /* a(n-1) initially a1 */

    for (idx_t i = (idx_t)1; i < n; ++i)
    {
        seq_t i_seq = (seq_t)i;
        seq_t two_n_plus1 = i_seq * (seq_t)2 + (seq_t)1;
        seq_t n2 = i_seq * i_seq;
        seq_t n4 = n2 * n2;
        seq_t term = (seq_t)3 * two_n_plus1 * a_nm1 + n4 * a_nm2;

        a_nm2 = a_nm1;
        a_nm1 = term;
    }

    return a_nm1;
}
