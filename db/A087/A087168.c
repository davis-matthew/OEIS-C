/*
 * A087168  Expansion of (1 + 2*x)/(1 + 3*x + 4*x^2).
 * https://oeis.org/A087168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -3*a(n-1) - 4*a(n-2), a(0) = 1, a(1) = -1
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 32, i64 n <= 63, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A087168 */
seq_t a(idx_t n)
{
    /* Handle initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)(-1);

    seq_t a_prevprev = (seq_t)1;   /* a(0) */
    seq_t a_prev     = (seq_t)(-1);/* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = -3*a(i-1) - 4*a(i-2) */
        a_curr = -( (seq_t)3 * a_prev + (seq_t)4 * a_prevprev );
        a_prevprev = a_prev;
        a_prev = a_curr;
    }
    return a_prev; /* a(n) */
}
