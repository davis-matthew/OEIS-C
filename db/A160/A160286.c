/*
 * A160286  Numerator of Hermite(n, 24/29).
 * https://oeis.org/A160286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 24/29)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=48;  for n>=1:  a(n+1) = 48*a(n) - 1682*n*a(n-1) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n == 1)
        return (seq_t)48;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)48;  /* a(1) */
    seq_t a_n;

    for (idx_t i = 1; i < n; ++i) {
        /* a(n+1) = 48*a(n) - 1682*n*a(n-1) */
        seq_t coeff = (seq_t)1682 * (seq_t)i;
        a_n = (seq_t)48 * a_nm1 - coeff * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
