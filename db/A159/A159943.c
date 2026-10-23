/*
 * A159943  Numerator of Hermite(n, 19/23).
 * https://oeis.org/A159943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = 38
   a(n) = 38*a(n-1) - 1058*(n-1)*a(n-2)   (n >= 2)
   Derived from Hermite polynomials: a(n) = 23^n * H_n(19/23)   */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)38;

    seq_t a_nm2 = (seq_t)1;   /* a(0)   */
    seq_t a_nm1 = (seq_t)38;  /* a(1)   */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term = ((seq_t)1058) * (seq_t)(i - 1) * a_nm2;
        seq_t a_n  = ((seq_t)38) * a_nm1 - term;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
