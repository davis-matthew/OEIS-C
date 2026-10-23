/*
 * A159858  Numerator of Hermite(n, 1/23).
 * https://oeis.org/A159858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 2*a(n-1) - 1058*(n-1)*a(n-2), a(0) = 1, a(1) = 2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 1;
    if (n == 1u)
        return 2;

    seq_t a0 = 1;
    seq_t a1 = 2;
    seq_t an;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term = (seq_t)1058 * (seq_t)(i - 1) * a0;
        an = 2 * a1 - term;
        a0 = a1;
        a1 = an;
    }

    return an;
}
