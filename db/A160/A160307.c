/*
 * A160307  Numerator of Hermite(n, 9/31).
 * https://oeis.org/A160307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n+2) = -1922*(n+1)*a(n) + 18*a(n+1) Initial values: a(0) = 1,
 *            a(1) = 18
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t a0 = (seq_t)1;  /* a(n)   */
    seq_t a1 = (seq_t)18; /* a(n+1) */

    for (idx_t i = 0; i + 2 <= n; ++i) {
        seq_t term1 = (seq_t)18 * a1;
        seq_t term2 = (seq_t)1922 * ((seq_t)(i + 1)) * a0;
        seq_t a2   = term1 - term2;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
