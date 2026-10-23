/*
 * A100334  An inverse Catalan transform of Fibonacci(2n).
 * https://oeis.org/A100334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 2*a(n-3) - a(n-4) with initial
 *            values a(0)=0, a(1)=1, a(2)=2, a(3)=2
 * Range:     i8 n <= 10, i16 n <= 24, i32 n <= 45, i64 n <= 91, i128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;
    if (n == (idx_t)3) return (seq_t)2;

    /* Use recurrence for n >= 4 */
    seq_t a0 = (seq_t)0;  /* a(n-4) */
    seq_t a1 = (seq_t)1;  /* a(n-3) */
    seq_t a2 = (seq_t)2;  /* a(n-2) */
    seq_t a3 = (seq_t)2;  /* a(n-1) */
    seq_t a4;             /* a(n)   */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        a4 = (seq_t)3 * a3 - (seq_t)4 * a2 + (seq_t)2 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a4;
}
