/*
 * A015610  a(n) = 12*a(n-1) + 5*a(n-2) for n >= 2, a(0) = 0, a(1) = 1.
 * https://oeis.org/A015610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) + 5*a(n-2) for n >= 2, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_nm2 = (seq_t)0;      /* a(n-2) */
    seq_t a_nm1 = (seq_t)1;      /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        a_n = (seq_t)12 * a_nm1 + (seq_t)5 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
