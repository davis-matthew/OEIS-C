/*
 * A182827  E.g.f. 1/sqrt(1+2x+4x^2).
 * https://oeis.org/A182827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(2*n-1)*a(n-1)-4*(n-1)^2*a(n-2), * a(0)=1, a(1)=-1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)-1;

    seq_t a_nm2 = (seq_t)1;      /* a(0) */
    seq_t a_nm1 = (seq_t)-1;     /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t im1 = (seq_t)(i - 1);
        seq_t term1 = ((seq_t)2 * (seq_t)i - (seq_t)1) * a_nm1;          /* (2*i-1)*a(i-1) */
        seq_t term2 = ((seq_t)4 * im1 * im1) * a_nm2;                    /* 4*(i-1)^2*a(i-2) */
        seq_t curr   = -(term1 + term2);                                /* a(i) */
        a_nm2 = a_nm1;
        a_nm1 = curr;
    }
    return a_nm1;
}
