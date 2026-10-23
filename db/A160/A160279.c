/*
 * A160279  Numerator of Hermite(n, 17/29).
 * https://oeis.org/A160279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 17/29). Equivalent recurrence: a(0) =
 *            1, a(1) = 34, a(n+1) = 34 * a(n) - 1682 * n * a(n-1) (for n >=
 *            1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)34;

    /* Use recurrence to compute up to the requested index. */
    seq_t prev = (seq_t)1;          /* a(0)   */
    seq_t cur  = (seq_t)34;         /* a(1)   */
    for (idx_t i = 1; i < n; ++i)   /* i = 1 .. n-1 */
    {
        /* next = 34*cur - 1682*i*prev */
        seq_t term = (seq_t)1682 * (seq_t)i * prev;
        seq_t next = (seq_t)34 * cur - term;
        prev = cur;
        cur = next;
    }
    return cur;
}
