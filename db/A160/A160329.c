/*
 * A160329  Numerator of Hermite(n, 21/31).
 * https://oeis.org/A160329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerator of Hermite(n, 21/31).
   Recurrence (derived from H_n(x)=2x H_{n-1}-2(n-1) H_{n-2},
   with x=21/31 and scaling by 31^n):
   a(0) = 1
   a(1) = 42
   a(n) = 42*a(n-1) - 1922*(n-1)*a(n-2)   for n >= 2
*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)42;

    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)42;         /* a(1) */
    seq_t ai;                     /* current a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* term1 = 42 * a_{i-1} */
        seq_t term1 = (seq_t)42 * a1;
        /* term2 = 1922 * (i-1) * a_{i-2} */
        seq_t term2 = (seq_t)1922 * (seq_t)(i - 1) * a0;

        ai = term1 - term2;

        /* shift for next iteration */
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
