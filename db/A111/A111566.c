/*
 * A111566  a(n) = ((1+sqrt(8))*(2+sqrt(2))^n + (1-sqrt(8))*(2-sqrt(2))^n)/2.
 * https://oeis.org/A111566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2), a(0) = 1, a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)6;   /* a(1) */
    seq_t a_cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        a_cur = (seq_t)4 * a_nm1 - (seq_t)2 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
    return a_cur;
}
