/*
 * A166035  a(n) = (3^n+6*(-4)^n)/7.
 * https://oeis.org/A166035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = -3
   a(n) = -a(n-1) + 12*a(n-2)   for n > 1
   Equivalent closed form: a(n) = (3^n + 6*(-4)^n)/7
*/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-3;

    seq_t a_nm2 = (seq_t)1;           /* a(n-2) */
    seq_t a_nm1 = (seq_t)-3;          /* a(n-1) */
    seq_t a_n   = (seq_t)0;           /* a(n) */

    for (idx_t k = 2; k <= n; ++k)
    {
        a_n = -(a_nm1) + (seq_t)12 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
