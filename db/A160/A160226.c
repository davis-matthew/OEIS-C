/*
 * A160226  Numerator of Hermite(n, 3/29).
 * https://oeis.org/A160226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 3/29) Recurrence: a(0) = 1, a(1) = 6
 *            a(n) = 6*a(n-1) - 1682*(n-1)*a(n-2) for n >= 2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of sequence A160226.
   The result is returned as a `seq_t`. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)6;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute a(i) = 6*a(i-1) - 1682*(i-1)*a(i-2) */
        seq_t mult = (seq_t)1682 * (seq_t)(i - 1);
        cur = (seq_t)6 * prev1 - mult * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
