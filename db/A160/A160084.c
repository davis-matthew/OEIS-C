/*
 * A160084  Numerator of Hermite(n, 25/26).
 * https://oeis.org/A160084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   * a(0) = 1 * a(1) = 25 * a(n+1) = 25 * a(n) - 338 * n * a(n-1)
 *            (for n ≥ 1) * This follows from a(n) = 13^n * Hermite(n, 25/26)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special‑case the first two values. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)25;

    /* a_prev holds a(k-1), a_curr holds a(k). */
    seq_t a_prev = (seq_t)1;   /* a(0) */
    seq_t a_curr = (seq_t)25;  /* a(1) */

    /* Compute successive values up to a(n). */
    for (idx_t k = 1; k < n; ++k) {
        /* 338 * k fits in a standard integer for all intended data sizes. */
        idx_t coeff = 338 * k;                 /* unsigned type */
        seq_t term   = (seq_t)coeff * a_prev;  /* 338*k*a(k-1) */
        seq_t next   = (seq_t)25 * a_curr - term; /* 25*a(k) - 338*k*a(k-1) */

        /* Shift for next iteration. */
        a_prev = a_curr;
        a_curr = next;
    }

    return a_curr;
}
