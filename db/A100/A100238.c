/*
 * A100238  G.f. A(x) satisfies: 2^n + 1 = Sum_{k=0..n} [x^k] A(x)^n for n>=1.
 * https://oeis.org/A100238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-2*(2*n-3)*a(n-1) + 4*(n-3)*a(n-2))/n, with a(0)=1,
 *            a(1)=2, a(2)=-2
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)-2;
    seq_t a_nm2 = (seq_t)2;        /* a(1) */
    seq_t a_nm1 = (seq_t)-2;       /* a(2) */
    seq_t a_cur = 0;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t term1 = -2 * ((seq_t)(2 * i - 3)) * a_nm1;
        seq_t term2 = 4 * ((seq_t)(i - 3)) * a_nm2;
        a_cur = (term1 + term2) / (seq_t)i;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
    return a_cur;
}
