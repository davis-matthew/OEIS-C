/*
 * A159621  Numerator of Hermite(n, 4/19).
 * https://oeis.org/A159621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 8*a(n-1) - 722*(n-1)*a(n-2) (with a(0)=1, a(1)=8)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)8;

    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)8;          /* a(1) */
    seq_t an = 0;

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)8 * a1 - (seq_t)722 * (seq_t)(i - 1) * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
