/*
 * A160139  Numerator of Hermite(n, 11/27).
 * https://oeis.org/A160139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27^n * Hermite(n, 11/27). Recurrence: a(0)=1, a(1)=22.
 *            For n ≥ 1, a(n+1) = 22*a(n) – 1458*n*a(n-1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t prevprev = (seq_t)1;          /* a(0) */
    seq_t prev     = (seq_t)22;         /* a(1) */
    if (n == 1) return prev;
    for (idx_t i = 1; i < n; ++i) {    /* compute a(i+1) using a(i) and a(i-1) */
        seq_t term = (seq_t)1458 * (seq_t)i * prevprev;
        seq_t next = (seq_t)22 * prev - term;
        prevprev = prev;
        prev = next;
    }
    return prev;
}
