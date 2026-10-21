/*
 * A078028  Expansion of (1-x)/(1-x^2+2*x^3).
 * https://oeis.org/A078028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=-1, a(2)=1; for n≥3, a(n)=a(n-2)-2*a(n-3)
 * Range:     i8 n <= 12, i16 n <= 25, i32 n <= 52, i64 n <= 105,
 *            i128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)(-1);
    if (n == 2) return (seq_t)1;

    /* Maintain a(i-3), a(i-2), a(i-1) */
    seq_t m3 = (seq_t)1;    /* a(0) */
    seq_t m2 = (seq_t)(-1); /* a(1) */
    seq_t m1 = (seq_t)1;    /* a(2) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = m2 - (seq_t)2 * m3;   /* a(i) = a(i-2) - 2*a(i-3) */
        m3 = m2;
        m2 = m1;
        m1 = ai;
    }
    return m1;   /* a(n) */
}
