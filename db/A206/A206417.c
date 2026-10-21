/*
 * A206417  (5*F(n)+3*L(n)-8)/2.
 * https://oeis.org/A206417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)-a(n-3), a(1)=0, a(2)=3, a(3)=7
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)7;

    seq_t a_nm3 = (seq_t)0;  /* a(n-3)  */
    seq_t a_nm2 = (seq_t)3;  /* a(n-2)  */
    seq_t a_nm1 = (seq_t)7;  /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t a_n = a_nm1 * ((seq_t)2) - a_nm3; /* a(n) = 2*a(n-1)-a(n-3) */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
