/*
 * A190984  a(n) = 9*a(n-1) - 7*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 7*a(n-2) Initial values: a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_nm2 = (seq_t)0;          /* a(n-2) */
    seq_t a_nm1 = (seq_t)1;          /* a(n-1) */
    seq_t a_n   = (seq_t)0;          /* current value */

    for (idx_t i = 2; i <= n; ++i) {
        a_n   = (seq_t)9 * a_nm1 - (seq_t)7 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
