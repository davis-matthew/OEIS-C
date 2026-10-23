/*
 * A159545  Numerator of Hermite(n, 1/18).
 * https://oeis.org/A159545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 9^n * Hermite(n,1/18)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;

    seq_t a0 = 1;
    seq_t a1 = 1;
    seq_t a2;
    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = a(i-1) - 162 * (i-1) * a(i-2) */
        a2 = a1 - ((seq_t)162 * (seq_t)(i - 1) * a0);
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
