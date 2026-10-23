/*
 * A159659  Numerator of Hermite(n, 7/20).
 * https://oeis.org/A159659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 7*a(n-1) - 200*(n-1)*a(n-2) a(0) = 1, a(1) = 7
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t a0 = (seq_t)1;     /* a(0) */
    seq_t a1 = (seq_t)7;     /* a(1) */
    seq_t an;                /* current term */

    for (idx_t i = 2; i <= n; ++i) {
        an  = (seq_t)7 * a1;
        an -= ((seq_t)200 * (seq_t)(i - 1)) * a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
