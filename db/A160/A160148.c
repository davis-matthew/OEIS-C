/*
 * A160148  Numerator of Hermite(n, 20/27).
 * https://oeis.org/A160148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27^n * Hermite(n, 20/27) Recurrence derived from
 *            Hermite's recurrence: a(0) = 1 a(1) = 40 a(n+1) = 40*a(n) -
 *            1458*n*a(n-1) for n ≥ 1
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)40;

    seq_t prevprev = (seq_t)1;   /* a(0) */
    seq_t prev     = (seq_t)40;  /* a(1) */

    for (idx_t i = 1; i < n; ++i) {
        seq_t coeff = (seq_t)1458 * (seq_t)i;
        seq_t term  = coeff * prevprev;          /* 1458 * i * a(n-1) */
        seq_t next  = (seq_t)40 * prev - term;   /* a(n+1) = 40*a(n) - 1458*i*a(n-1) */
        prevprev = prev;
        prev     = next;
    }

    return prev;   /* a(n) */
}
