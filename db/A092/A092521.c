/*
 * A092521  a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3), with a(1) = 1, a(2) = 8, a(3)
 *          = 56.
 * https://oeis.org/A092521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3) with initial values a(1)=1,
 *            a(2)=8, a(3)=56, offset 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A092521. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)56;

    seq_t a_nm3 = (seq_t)1;    /* a(n-3) for current iteration   */
    seq_t a_nm2 = (seq_t)8;    /* a(n-2) for current iteration   */
    seq_t a_nm1 = (seq_t)56;   /* a(n-1) for current iteration   */

    seq_t a_n;                 /* current value to be computed   */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)8 * a_nm1 - (seq_t)8 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
