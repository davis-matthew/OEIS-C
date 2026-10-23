/*
 * A174443  Expansion of g.f. x/(1 + 4*x - 8*x^2).
 * https://oeis.org/A174443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*a(n-1) + 8*a(n-2), a(0)=0, a(1)=1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 13, i64 n <= 26, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_nm2 = (seq_t)0;     /* a(n-2)  */
    seq_t a_nm1 = (seq_t)1;     /* a(n-1)  */
    seq_t a_n;                  /* current a(n) */
    for (idx_t i = 2; i <= n; ++i) {
        a_n = ((seq_t)-4) * a_nm1 + ((seq_t)8) * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
