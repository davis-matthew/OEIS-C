/*
 * A159826  Numerator of Hermite(n, 7/22).
 * https://oeis.org/A159826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 11^n * Hermite(n, 7/22)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 7,
 * a(n) = 7 * a(n-1) - 242 * (n-1) * a(n-2)   for n ≥ 2 */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)7;
    }

    /* Use a simple iterative recurrence. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)7;   /* a(1) */
    seq_t a2;              /* temporary storage for a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 7 * a(i-1) - 242 * (i-1) * a(i-2) */
        a2 = (seq_t)7 * a1;
        seq_t sub = (seq_t)242 * (seq_t)(i - 1) * a0;
        a2 -= sub;

        /* shift indices for next iteration */
        a0 = a1;
        a1 = a2;
    }

    return a1;
}
