/*
 * A108898  a(n+3) = 3*a(n+2) - 2*a(n), a(0) = -1, a(1) = 1, a(2) = 3.
 * https://oeis.org/A108898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n+3) = 3*a(n+2) - 2*a(n), with a(0) = -1, a(1) = 1, a(2) = 3
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 21, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial values directly */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Iterative computation of the linear recurrence */
    seq_t a0 = (seq_t)-1;   /* a(n-3)   */
    seq_t a1 = (seq_t)1;    /* a(n-2)   */
    seq_t a2 = (seq_t)3;    /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t an = (seq_t)3 * a2 - (seq_t)2 * a0; /* a(n) = 3*a(n-1) - 2*a(n-3) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
