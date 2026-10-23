/*
 * A159705  Numerator of Hermite(n, 1/21).
 * https://oeis.org/A159705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 2*a(n-1) - 882*(n-1)*a(n-2); a(0)=1, a(1)=2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_nm2 = (seq_t)1; /* a(n-2) */
    seq_t a_nm1 = (seq_t)2; /* a(n-1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        a_curr = (seq_t)2 * a_nm1 - (seq_t)882 * (seq_t)(i - 1) * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_curr;
    }

    return a_nm1;
}
