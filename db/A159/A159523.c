/*
 * A159523  Numerator of Hermite(n, 5/16).
 * https://oeis.org/A159523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 5, a(n) = 5*a(n-1) - 128*(n-1)*a(n-2)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a_nm2 = (seq_t)1;  /* a(0) */
    seq_t a_nm1 = (seq_t)5;  /* a(1) */
    seq_t a_n   = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        a_n = (seq_t)5 * a_nm1 - (seq_t)128 * (i - 1) * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
