/*
 * A055273  a(n) = 3*a(n-1) - a(n-2) with a(0) = 1, a(1) = 8.
 * https://oeis.org/A055273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2), a(0) = 1, a(1) = 8
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    seq_t a_nm2 = (seq_t)1;
    seq_t a_nm1 = (seq_t)8;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a_n = a_nm1 * (seq_t)3 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
