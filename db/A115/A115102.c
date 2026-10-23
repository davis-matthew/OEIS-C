/*
 * A115102  a(0)=2, a(1)=8, a(n) = a(n-1) + 2*a(n-2).
 * https://oeis.org/A115102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-1) + 2*a(n-2), a(0)=2, a(1)=8
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)8;

    seq_t a_nm2 = (seq_t)2; /* a(n-2) */
    seq_t a_nm1 = (seq_t)8; /* a(n-1) */
    seq_t a_n   = a_nm1;

    for (idx_t i = 2; i <= n; ++i) {
        a_n = a_nm1 + (a_nm2 << 1); /* 2*a_nm2 */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
