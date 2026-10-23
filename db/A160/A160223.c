/*
 * A160223  Numerator of Hermite(n, 27/28).
 * https://oeis.org/A160223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 14^n * Hermite(n, 27/28). Recurrence: a(0) = 1, a(1) =
 *            27; for n ≥ 2: a(n) = 27*a(n-1) - 392*(n-1)*a(n-2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)27;

    seq_t a0 = (seq_t)1;   /* a(n-2)   */
    seq_t a1 = (seq_t)27;  /* a(n-1)   */

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 27*a1 - 392*(i-1)*a0 */
        seq_t term = (seq_t)27 * a1 - (seq_t)392 * (seq_t)(i - 1) * a0;
        a0 = a1;
        a1 = term;
    }
    return a1;
}
