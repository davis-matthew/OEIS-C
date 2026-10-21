/*
 * A219233  Alternating row sums of Riordan triangle A110162.
 * https://oeis.org/A219233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = -3, a(2) = 7, a(n) = -3*a(n-1) - a(n-2) for n
 *            >= 3
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 22, i64 n <= 45, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-3;
    if (n == 2) return (seq_t)7;

    seq_t a_prevprev = (seq_t)-3; /* a(1) */
    seq_t a_prev     = (seq_t)7;  /* a(2) */
    seq_t a_cur;

    for (idx_t i = 3; i <= n; ++i) {
        a_cur = - (seq_t)3 * a_prev - a_prevprev;
        a_prevprev = a_prev;
        a_prev = a_cur;
    }
    return a_cur;
}
