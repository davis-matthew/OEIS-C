/*
 * A078061  Expansion of (1-x)/(1+2*x+x^3).
 * https://oeis.org/A078061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) - a(n-3), a(0)=1, a(1)=-3, a(2)=6
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 26, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)-3;
    if (n == 2u) return (seq_t)6;

    seq_t a_nm3 = (seq_t)1;   /* a(n-3)   */
    seq_t a_nm2 = (seq_t)-3;  /* a(n-2)   */
    seq_t a_nm1 = (seq_t)6;   /* a(n-1)   */
    seq_t a_n   = (seq_t)0;   /* current term */

    for (idx_t i = 3u; i <= n; ++i) {
        a_n = - (seq_t)2 * a_nm1 - a_nm3; /* a_n = -2*a(n-1) - a(n-3) */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
