/*
 * A160012  Numerator of Hermite(n, 8/25).
 * https://oeis.org/A160012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 8/25)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 16, a(n+1) = 16*a(n) - 1250*n*a(n-1) for n >= 1. */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 16;

    seq_t a_prevprev = 1;   /* a(0) */
    seq_t a_prev = 16;      /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = 16*a(i-1) - 1250*(i-1)*a(i-2) */
        a_curr = 16 * a_prev;
        a_curr -= ((seq_t)1250 * (seq_t)(i - 1)) * a_prevprev;
        a_prevprev = a_prev;
        a_prev = a_curr;
    }

    return a_prev;
}
