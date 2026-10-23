/*
 * A159520  Numerator of Hermite(n, 14/15).
 * https://oeis.org/A159520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 28*a(n-1) - 450*(n-1)*a(n-2); a(0)=1, a(1)=28
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    if (n == 1) {
        return 28;
    }

    seq_t a0 = 1;            /* a(n-2) : start with a(0) */
    seq_t a1 = 28;           /* a(n-1) : start with a(1) */
    seq_t an;                /* temporary for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t im1 = (seq_t)(i - 1);           /* (n-1) as seq_t */
        an = 28 * a1 - (seq_t)450 * im1 * a0; /* recurrence */
        a0 = a1;          /* shift window: a(n-2) <- a(n-1) */
        a1 = an;          /* a(n-1) <- a(n)   */
    }
    return a1;   /* a(n) after loop ends */
}
