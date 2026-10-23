/*
 * A109115  a(n) = 4*a(n-1) + 3*a(n-2), a(0)=1, a(1)=6.
 * https://oeis.org/A109115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 3*a(n-2), a(0) = 1, a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
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
    seq_t a_n   = a_nm1;      /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        a_n = a_nm1 * (seq_t)4 + a_nm2 * (seq_t)3;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
