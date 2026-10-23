/*
 * A160197  Numerator of Hermite(n, 15/28).
 * https://oeis.org/A160197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 15*a(n-1) - 392*(n-1)*a(n-2), a(0)=1, a(1)=15
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)15;

    /* Iterative computation using the recurrence. */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)15;  /* a(n-1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute 15 * a(n-1) */
        seq_t term1 = (seq_t)15 * a_nm1;

        /* Compute 392 * (i-1) * a(n-2) */
        seq_t coeff = (seq_t)(392 * (i - 1));
        seq_t term2 = coeff * a_nm2;

        /* a(n) = term1 - term2 */
        a_n = term1 - term2;

        /* Shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
