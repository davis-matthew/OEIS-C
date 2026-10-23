/*
 * A176368  x-values in the solution to x^2 - 65*y^2 = 1.
 * https://oeis.org/A176368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 258*a(n-1) - a(n-2), with a(1)=1, a(2)=129
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the value is undefined in OEIS terms; return 0. */
    if (n == 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;

    if (n == 2)
        return (seq_t)129;

    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)129; /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)258 * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1; /* a(n) */
}
