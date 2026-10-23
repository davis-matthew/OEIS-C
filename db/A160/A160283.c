/*
 * A160283  Numerator of Hermite(n, 21/29).
 * https://oeis.org/A160283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 21/29)
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=42, a(n+1)=42*a(n)-1682*n*a(n-1) */

seq_t a(idx_t n)
{
    seq_t prev2 = 1;            /* a(0) */
    if (n == 0) return prev2;

    seq_t prev1 = 42;           /* a(1) */
    if (n == 1) return prev1;

    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t mult = (seq_t)(i - 1);
        cur = 42 * prev1 - 1682 * mult * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
