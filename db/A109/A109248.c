/*
 * A109248  Expansion of (1-x-2*x^2)/(1-x^2+x^3).
 * https://oeis.org/A109248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-2) - a(n-3), initial values a(0) = 1, a(1) = a(2) =
 *            -1
 * Range:     i8 n <= 22, i16 n <= 41, i32 n <= 81, i64 n <= 160,
 *            i128 n <= 317, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               return (seq_t)1;
    if (n == 1 || n == 2)     return (seq_t)-1;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)-1;  /* a(1) */
    seq_t a2 = (seq_t)-1;  /* a(2) */
    seq_t a3;

    for (idx_t i = 3; i <= n; ++i) {
        a3 = a1 - a0;     /* a(i) = a(i-2) - a(i-3) */
        a0 = a1;          /* shift the window */
        a1 = a2;
        a2 = a3;
    }
    return a2;            /* a(n) is stored in a2 after loop */
}
