/*
 * A160270  Numerator of Hermite(n, 16/29).
 * https://oeis.org/A160270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 16/29) Recurrence derived from Hermite
 *            recurrence: a(0) = 1 a(1) = 32 a(n) = 32*a(n-1) -
 *            1682*(n-1)*a(n-2) for n >= 2
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)32;

    seq_t prev2 = (seq_t)1;       /* a(0) */
    seq_t prev1 = (seq_t)32;      /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = 32 * prev1;
        seq_t term2 = 1682 * (seq_t)(i - 1);
        term2 *= prev2;
        next = term1 - term2;

        prev2 = prev1;
        prev1 = next;
    }

    return prev1;
}
