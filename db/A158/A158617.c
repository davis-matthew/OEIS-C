/*
 * A158617  a(n) = Hermite(n, 16).
 * https://oeis.org/A158617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 32*a(n-1) - 2*(n-1)*a(n-2) Initial values: a(0)=1,
 *            a(1)=32 Recurrence: a(n) = 32*a(n-1) - 2*(n-1)*a(n-2) for n >= 2
 *            Hermite polynomial H_n(16)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)32;
    }

    seq_t a_prev_prev = (seq_t)1;   /* a(0) */
    seq_t a_prev      = (seq_t)32;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t i_minus_1 = (seq_t)(i - 1);
        /* term = 32*a_prev - 2*(i-1)*a_prev_prev */
        seq_t term = (seq_t)32 * a_prev - (seq_t)2 * i_minus_1 * a_prev_prev;
        a_prev_prev = a_prev;
        a_prev = term;
    }

    return a_prev;
}
