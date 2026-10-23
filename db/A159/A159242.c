/*
 * A159242  Numerator of Hermite(n, 7/9).
 * https://oeis.org/A159242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A159242
   Formula: a(0) = 1, a(1) = 14
   Recurrence: a(n) = 14*a(n-1) - 162*(n-1)*a(n-2)   for n ≥ 2
   (equivalent to a(n) = 9^n * Hermite(n, 7/9))
*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)14;

    seq_t a_nm2 = (seq_t)1;          /* a(0) */
    seq_t a_nm1 = (seq_t)14;         /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* mul = 162 * (i-1) expressed as seq_t */
        seq_t mul = (seq_t)(162 * (i - 1));

        /* recurrence: a(i) = 14*a(i-1) - mul * a(i-2) */
        a_curr = 14 * a_nm1 - mul * a_nm2;

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_curr;
    }
    return a_nm1;
}
