/*
 * A160152  Numerator of Hermite(n, 25/27).
 * https://oeis.org/A160152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27^n * Hermite(n, 25/27)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)50;

    /* a(0) and a(1) are the starting values. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)50;  /* a(1) */

    /* Use the recurrence:
       a(n+1) = 50 * a(n) - 1458 * n * a(n-1)             */
    for (idx_t j = 1; j < n; ++j) {
        seq_t term = (seq_t)1458 * (seq_t)j * a0;   /* 1458 * j * a(n-1) */
        seq_t cur  = (seq_t)50 * a1 - term;          /* compute a(n+1)   */
        a0 = a1;                                     /* shift for next step */
        a1 = cur;
    }
    return a1; /* a(n) */
}
