/*
 * A159882  Numerator of Hermite(n, 13/23).
 * https://oeis.org/A159882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 26*a(n-1) - 1058*(n-1)*a(n-2), a(0)=1, a(1)=26
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence derived from a(n) = 23^n * Hermite(n,13/23) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    seq_t prevprev = (seq_t)1;   /* a(0) */
    seq_t prev     = (seq_t)26;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)26 * prev
                    - (seq_t)1058 * (seq_t)(i - 1) * prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
