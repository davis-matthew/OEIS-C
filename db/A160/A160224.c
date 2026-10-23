/*
 * A160224  Numerator of Hermite(n, 1/29).
 * https://oeis.org/A160224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 1/29) Recurrence: a(0)=1, a(1)=2; a(n)
 *            = 2*a(n-1) - 1682*(n-1)*a(n-2) (for n≥2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Previous two terms of the sequence */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)2;   /* a(1) */
    seq_t a_i   = 0;          /* current term a(n) */

    /* Iterate using the recurrence relation */
    for (idx_t i = 2; i <= n; ++i) {
        /* term = 1682 * (i-1) * a(i-2) */
        seq_t term = (seq_t)1682 * (seq_t)(i - 1) * a_nm2;
        /* a(i) = 2*a(i-1) - term */
        a_i = (seq_t)2 * a_nm1 - term;
        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }

    return a_nm1;
}
