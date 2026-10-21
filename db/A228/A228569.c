/*
 * A228569  Binomial transform of A006497.
 * https://oeis.org/A228569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2), a(0) = 2, a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External types are supplied by the build system, no typedefs are needed here */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)5;

    seq_t a_nm2 = (seq_t)2; /* a(0) */
    seq_t a_nm1 = (seq_t)5; /* a(1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i)
    {
        a_n = a_nm1 * (seq_t)5 - a_nm2 * (seq_t)3; /* a(i) = 5*a(i-1) - 3*a(i-2) */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
