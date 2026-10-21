/*
 * A078036  Expansion of (1-x)/(1+2*x^2+x^3).
 * https://oeis.org/A078036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = -1, a(2) = -2, a(n) = -2 * a(n-2) - a(n-3) for
 *            n ≥ 3
 * Range:     i8 n <= 12, i16 n <= 27, i32 n <= 54, i64 n <= 111,
 *            i128 n <= 222, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)-2;

    /* Use a sliding window of the last three computed values:
       a0 = a(i-3), a1 = a(i-2), a2 = a(i-1). */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)-1;  /* a(1) */
    seq_t a2 = (seq_t)-2;  /* a(2) */
    seq_t an;
    idx_t i;

    for (i = 3; i <= n; ++i) {
        /* a(i) = -2 * a(i-2) - a(i-3) */
        an = -( (seq_t)2 * a1 ) - a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2; /* a(n) now stored in a2 */
}
