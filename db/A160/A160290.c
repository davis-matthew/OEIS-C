/*
 * A160290  Numerator of Hermite(n, 28/29).
 * https://oeis.org/A160290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 28/29) Recurrence: a(0)=1, a(1)=56,
 *            a(n+1)=56*a(n)-1682*n*a(n-1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)56;
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)56;  /* a(1) */
    seq_t a_n   = a_nm1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t k = i-1; /* current n in recurrence */
        a_n = (seq_t)56 * a_nm1 - (seq_t)1682 * (seq_t)k * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
