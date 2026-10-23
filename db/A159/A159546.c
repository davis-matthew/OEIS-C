/*
 * A159546  Numerator of Hermite(n, 5/18).
 * https://oeis.org/A159546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 5*a(n-1) - 162*(n-1)*a(n-2), a(0) = 1, a(1) = 5
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;

    seq_t a_nm2 = 1; /* a(0) */
    seq_t a_nm1 = 5; /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t factor = (seq_t)162 * (seq_t)(i - 1);
        seq_t cur = 5 * a_nm1 - factor * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
