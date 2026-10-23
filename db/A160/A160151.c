/*
 * A160151  Numerator of Hermite(n, 23/27).
 * https://oeis.org/A160151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27^n * Hermite(n, 23/27) = 46*a(n-1) - 1458*(n-1)*a(n-2),
 *            with a(0)=1, a(1)=46
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)46;

    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)46; /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff = (seq_t)1458 * (seq_t)(i - 1);
        an = (seq_t)46 * a1 - coeff * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
