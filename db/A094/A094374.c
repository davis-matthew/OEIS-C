/*
 * A094374  a(n) = (3^n-1)/2 + 2^n.
 * https://oeis.org/A094374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3), with a(0)=1, a(1)=3,
 *            a(2)=8
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
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

    seq_t a_nm3 = (seq_t)1;   /* a(i-3)  */
    seq_t a_nm2 = (seq_t)3;   /* a(i-2)  */
    seq_t a_nm1 = (seq_t)8;   /* a(i-1)  */
    seq_t a_cur;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t temp = (seq_t)6 * a_nm1 + (seq_t)6 * a_nm3;
        a_cur = temp - (seq_t)11 * a_nm2;

        /* shift the window */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
    return a_nm1;
}
