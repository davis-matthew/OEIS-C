/*
 * A159280  Numerator of Hermite(n, 1/11).
 * https://oeis.org/A159280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 11^n * Hermite(n, 1/11) Recurrence: a(0) = 1, a(1) = 2,
 *            a(n) = 2*a(n-1) - 242*(n-1)*a(n-2) for n ≥ 2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)2;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)2;   /* a(n-1) */

    for (idx_t i = 2U; i <= n; ++i) {
        seq_t mul = (seq_t)242 * ((seq_t)(i - 1U) * a_nm2);
        seq_t cur = (seq_t)2 * a_nm1 - mul;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1;
}
