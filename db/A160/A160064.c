/*
 * A160064  Numerator of Hermite(n, 19/25).
 * https://oeis.org/A160064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 19/25). Recurrence: a(0) = 1 a(1) = 38
 *            a(n) = 38*a(n-1) - 1250*(n-1)*a(n-2) for n >= 2
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)38;

    seq_t a_prev_prev = (seq_t)1;   /* a(0) */
    seq_t a_prev     = (seq_t)38;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        idx_t coeff_idx = (idx_t)1250 * (i - (idx_t)1);
        seq_t coeff = (seq_t)coeff_idx;
        seq_t a_curr = (seq_t)38 * a_prev - coeff * a_prev_prev;
        a_prev_prev = a_prev;
        a_prev = a_curr;
    }
    return a_prev;
}
