/*
 * A124342  Expansion of (1+x)/(1+2x-2x^3).
 * https://oeis.org/A124342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) + 2*a(n-3) with initial values a(0) = 1, a(1) =
 *            -1, a(2) = 2 This recurrence follows from the generating
 *            function (1+x)/(1+2x-2x^3). It yields the integer sequence
 *            A124342
 * Range:     i8 n <= 13, i16 n <= 24, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)2;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)-1;  /* a(1) */
    seq_t a2 = (seq_t)2;   /* a(2) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = (-2 * a2) + (2 * a0);  /* a(i) = -2*a(i-1) + 2*a(i-3) */
        a0 = a1;   /* shift window: discard oldest */
        a1 = a2;
        a2 = ai;   /* now holds a(i) for next iteration */
    }
    return a2;
}
