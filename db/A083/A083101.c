/*
 * A083101  a(n) = 2*a(n-1) + 10*a(n-2), with a(0) = 1, a(1) = 12.
 * https://oeis.org/A083101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 10*a(n-2); a(0) = 1, a(1) = 12
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)12;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)12;  /* a(n-1) */
    seq_t a_n = 0;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        a_n = a_nm1 * (seq_t)2 + a_nm2 * (seq_t)10;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
