/*
 * A160147  Numerator of Hermite(n, 19/27).
 * https://oeis.org/A160147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = 38
   a(n) = 38 * a(n-1) - 1458 * (n-1) * a(n-2)   (n >= 2)
   This implements the OEIS sequence A160147: the numerator of
   Hermite(n, 19/27).
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)38;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2), start with a(0) */
    seq_t a_nm1 = (seq_t)38;  /* a(n-1), start with a(1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = 38 * a(n-1) */
        seq_t term1 = (seq_t)38 * a_nm1;

        /* term2 = 1458 * (i-1) * a(n-2) */
        seq_t term2 = a_nm2 * (seq_t)(1458 * (i - 1));

        /* a(n) = term1 - term2 */
        a_n = term1 - term2;

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
