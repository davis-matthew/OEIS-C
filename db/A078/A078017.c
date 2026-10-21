/*
 * A078017  Expansion of (1-x)/(1-x+x^2+2*x^3).
 * https://oeis.org/A078017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-2) - 2*a(n-3) (for n ≥ 3) Initial values:
 *            a(0) = 1, a(1) = 0, a(2) = -1
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 48, i64 n <= 97, i128 n <= 196,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first three terms */
    seq_t t0 = 1;    /* a(0) */
    seq_t t1 = 0;    /* a(1) */
    seq_t t2 = -1;   /* a(2) */

    if (n == 0) return t0;
    if (n == 1) return t1;
    if (n == 2) return t2;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = t2 - t1 - 2 * t0;  /* a(i) = a(i-1) - a(i-2) - 2*a(i-3) */
        t0 = t1;
        t1 = t2;
        t2 = next;
    }
    return t2;   /* a(n) */
}
