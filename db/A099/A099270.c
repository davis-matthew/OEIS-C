/*
 * A099270  Unsigned member r=-12 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A099270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1)+13*a(n-2)-a(n-3), a(0)=0, a(1)=1, a(2)=12
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)12;

    seq_t a_nm3 = (seq_t)0;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)12;  /* a(n-1) */
    seq_t a_n;

    for (idx_t i = 3; i <= n; ++i) {
        /* a_n = 13*a_nm1 + 13*a_nm2 - a_nm3 */
        a_n = (seq_t)13 * a_nm1 + (seq_t)13 * a_nm2 - a_nm3;
        /* shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
