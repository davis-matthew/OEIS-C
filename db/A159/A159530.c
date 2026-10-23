/*
 * A159530  Numerator of Hermite(n, 2/17).
 * https://oeis.org/A159530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 17^n * Hermite(n,2/17) * Recurrence: a(0) = 1, a(1) = 4,
 *            a(n+1) = 4*a(n) - 578*n*a(n-1)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t a_prev = (seq_t)1; /* a(0) */
    seq_t a_cur  = (seq_t)4; /* a(1) */

    for (idx_t k = 1; k < n; ++k)
    {
        seq_t mult = (seq_t)578 * (seq_t)k;     /* 578 * k   */
        seq_t term = mult * a_prev;             /* 578*k*a(k-1) */
        seq_t a_next = (seq_t)4 * a_cur - term; /* 4*a(k) - term */
        a_prev = a_cur;
        a_cur  = a_next;
    }
    return a_cur;
}
