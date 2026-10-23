/*
 * A160287  Numerator of Hermite(n, 25/29).
 * https://oeis.org/A160287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 25/29)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=50; for n>=2: a(n) = 50*a(n-1) - 1682*(n-1)*a(n-2) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)50;

    seq_t a0 = (seq_t)1;        /* a(0) */
    seq_t a1 = (seq_t)50;       /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff = (seq_t)1682 * (seq_t)(i - 1);
        seq_t a2 = (seq_t)50 * a1 - coeff * a0;
        a0 = a1;
        a1 = a2;
    }

    return a1;
}
