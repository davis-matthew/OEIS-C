/*
 * A159709  Numerator of Hermite(n, 5/21).
 * https://oeis.org/A159709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 10, a(n) = 10*a(n-1) - 882*(n-1)*a(n-2) for n
 *            >= 2
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
        return (seq_t)10;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)10;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term   = (seq_t)10 * a1;
        seq_t mult   = (seq_t)(i - 1);
        mult *= (seq_t)882;
        mult *= a0;
        seq_t a = term - mult;
        a0 = a1;
        a1 = a;
    }

    return a1;
}
