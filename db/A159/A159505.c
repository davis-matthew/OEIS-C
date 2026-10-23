/*
 * A159505  Numerator of Hermite(n, 11/13).
 * https://oeis.org/A159505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 22*a(n-1) - 338*(n-1)*a(n-2)  (a(0) = 1, a(1) = 22)
 * that is the recurrence derived from the Hermite polynomial definition
 * a(n) = 13^n * H_n(11/13) with generating function exp(22*x - 169*x^2).
 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;

    seq_t prevprev = (seq_t)1; /* a(0) */
    seq_t prev     = (seq_t)22; /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* term = 338 * (i-1) * a(i-2) */
        seq_t term = (seq_t)338 * (seq_t)(i - 1) * prevprev;
        /* a(i) = 22 * a(i-1) - term */
        cur = (seq_t)22 * prev - term;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
