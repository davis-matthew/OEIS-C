/*
 * A052918  a(0) = 1, a(1) = 5, a(n+1) = 5*a(n) + a(n-1).
 * https://oeis.org/A052918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=5, a(n)=5*a(n-1)+a(n-2) for n≥2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)5;   /* a(n-1) */
    seq_t a_n   = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        a_n = a_nm1 * (seq_t)5 + a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
