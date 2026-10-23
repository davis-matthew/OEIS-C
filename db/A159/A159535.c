/*
 * A159535  Numerator of Hermite(n, 7/17).
 * https://oeis.org/A159535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 17^n * Hermite(n, 7/17)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=14, a(n+1)=14*a(n)-578*n*a(n-1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;

    seq_t a_prev2 = (seq_t)1; /* a(0) */
    seq_t a_prev1 = (seq_t)14; /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)14 * a_prev1
              - (seq_t)578 * (seq_t)(i - 1) * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }

    return cur;
}
