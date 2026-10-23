/*
 * A041061  Denominators of continued fraction convergents to sqrt(37).
 * https://oeis.org/A041061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 12*a(n-1) + a(n-2), a(0)=1, a(1)=12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)12;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t a_n = (seq_t)12 * a_nm1 + a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
