/*
 * A134581  a(n) = 4*a(n-1) - 7*a(n-2) + 6*a(n-3) - 3*a(n-4), starting with 0,
 *          1, 2, 3.
 * https://oeis.org/A134581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 20, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 7*a(n-2) + 6*a(n-3) - 3*a(n-4),
   a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 3 */

seq_t a(idx_t n)
{
    /* Handle the initial four values directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* We only need the last four values of the sequence to compute the next one. */
    seq_t v0 = (seq_t)0;  /* a(n-4) */
    seq_t v1 = (seq_t)1;  /* a(n-3) */
    seq_t v2 = (seq_t)2;  /* a(n-2) */
    seq_t v3 = (seq_t)3;  /* a(n-1) */
    seq_t val;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 4*a(i-1) - 7*a(i-2) + 6*a(i-3) - 3*a(i-4) */
        val = 4 * v3 - 7 * v2 + 6 * v1 - 3 * v0;
        /* Shift the window to the right. */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = val;
    }

    return v3;  /* v3 now holds a(n) */
}
