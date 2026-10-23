/*
 * A014983  a(n) = (1 - (-3)^n)/4.
 * https://oeis.org/A014983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 - (-3)^n)/4 Recurrence: a(0) = 0, a(1) = 1, a(n) =
 *            -2*a(n-1) + 3*a(n-2) for n > 1
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 41, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;

    /* iterate using the recurrence, keeping only the last two values */
    seq_t a0 = (seq_t)0;      /* a(n-2) */
    seq_t a1 = (seq_t)1;      /* a(n-1) */
    seq_t ai;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        ai = - (seq_t)2 * a1 + (seq_t)3 * a0;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
