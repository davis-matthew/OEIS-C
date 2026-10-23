/*
 * A159013  Numerator of Hermite(n, 6/7).
 * https://oeis.org/A159013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 7^n * Hermite(n, 6/7) Recurrence: a(0)=1, a(1)=12,
 *            a(n+1)=12*a(n) - 98*n*a(n-1) for n≥1
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 14, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    seq_t a_prev = (seq_t)1;       /* a(0) */
    seq_t a_curr = (seq_t)12;      /* a(1) */
    seq_t a_next;
    for (idx_t i = 1; i < n; ++i)  /* compute a(i+1) up to a(n) */
    {
        a_next = (seq_t)12 * a_curr - (seq_t)98 * (seq_t)i * a_prev;
        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;
}
