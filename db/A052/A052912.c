/*
 * A052912  Expansion of 1/(1-2*x-2*x^3).
 * https://oeis.org/A052912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) with a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* a(n-3), a(n-2), a(n-1) */
    seq_t a_nm3 = (seq_t)1;   /* a(0) */
    seq_t a_nm2 = (seq_t)2;   /* a(1) */
    seq_t a_nm1 = (seq_t)4;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + 2*a(i-3) */
        seq_t w = a_nm1 + a_nm1;   /* 2*a(i-1) */
        w += a_nm3 + a_nm3;       /* + 2*a(i-3) */

        /* prepare for next step: shift the window */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = w;
    }

    return a_nm1;
}
