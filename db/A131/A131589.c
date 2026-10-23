/*
 * A131589  Expansion of -(3+9*x+2*x^2)/((x+1)*(x^2+3*x+1)).
 * https://oeis.org/A131589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -4*a(n-1) - 4*a(n-2) - a(n-3) with initial values a(0) =
 *            -3, a(1) = 3, a(2) = -2
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 23, i64 n <= 46, i128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)(-3);
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)(-2);

    seq_t a0 = (seq_t)(-3);   /* a(n-3) */
    seq_t a1 = (seq_t)3;      /* a(n-2) */
    seq_t a2 = (seq_t)(-2);   /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = -4*a2 - 4*a1 - a0; /* recurrence */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
