/*
 * A160059  Numerator of Hermite(n, 13/25).
 * https://oeis.org/A160059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 13/25) Recurrence: a(0)=1, a(1)=26,
 *            a(n+1) = 26*a(n) - 1250*n*a(n-1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)26;
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)26;  /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        /* term = 1250 * i * a(i-1) */
        seq_t term = (seq_t)1250 * (seq_t)i * prev2;
        /* a(i+1) = 26*a(i) - term */
        seq_t next = (seq_t)26 * prev1 - term;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
