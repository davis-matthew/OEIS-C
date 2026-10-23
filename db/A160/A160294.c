/*
 * A160294  Numerator of Hermite(n, 13/30).
 * https://oeis.org/A160294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 15^n * Hermite(n, 13/30) * Recurrence: a(0) = 1, a(1) =
 *            13, * a(n+1) = 13*a(n) - 450*n*a(n-1) for n ≥ 1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 13;

    seq_t a_nm2 = 1;   /* a(n-1) when n=1 */
    seq_t a_nm1 = 13;  /* a(n)   when n=1 */
    seq_t a_n;
    for (idx_t i = 1; i < n; ++i)   /* i is current n-1 */
    {
        seq_t mul = 450 * (seq_t)i;           /* 450 * (n-1) */
        a_n = 13 * a_nm1 - mul * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
