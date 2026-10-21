/*
 * A094723  a(n) = Pell(n+2) - 2^n.
 * https://oeis.org/A094723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 2*a(n-3), a(0) = 1, a(1) = 3, a(2)
 *            = 8
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)8;

    seq_t a_nm3 = (seq_t)1;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)3;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)8;   /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = a_nm1 * (seq_t)4;
        an -= a_nm2 * (seq_t)3;
        an -= a_nm3 * (seq_t)2;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return a_nm1;
}
