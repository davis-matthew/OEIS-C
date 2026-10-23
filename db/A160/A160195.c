/*
 * A160195  Numerator of Hermite(n, 11/28).
 * https://oeis.org/A160195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 11*a(n-1) - 392*(n-1)*a(n-2) Initial values: a(0)=1,
 *            a(1)=11 This implements a(n) = 14^n * Hermite(n, 11/28) as an
 *            integer recurrence
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2)   */
    seq_t a_nm1 = (seq_t)11;  /* a(n-1)   */
    seq_t a_n   = 0;

    for (idx_t k = 2; k <= n; ++k) {
        a_n = (seq_t)11 * a_nm1
              - (seq_t)392 * (seq_t)(k - 1) * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
